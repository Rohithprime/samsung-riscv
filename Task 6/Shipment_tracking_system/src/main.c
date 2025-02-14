#include <ch32v00x.h>
#include <debug.h>
#include <stdlib.h>

/* MPU6050 Register Addresses */
#define MPU6050_ADDR 0x68
#define MPU6050_PWR_MGMT_1 0x6B
#define MPU6050_ACCEL_CONFIG 0x1C
#define MPU6050_ACCEL_XOUT_H 0x3B
#define MPU6050_WHO_AM_I 0x75

/* Tilt Detection Parameters */
#define TILT_THRESHOLD 15000  // Raw accelerometer threshold
#define I2C_TIMEOUT 1000      // Timeout for I2C operations

/* Global Variable */
int16_t accelX, accelY, accelZ;
uint8_t tiltDetected = 0;

/* Function Prototypes */
void GPIO_Config(void);
void MPU6050_Init(void);
void MPU6050_Read_Accel(void);
void Check_Tilt(void);
void I2C_WriteByte(uint8_t addr, uint8_t reg, uint8_t data);
uint8_t I2C_ReadByte(uint8_t addr, uint8_t reg);

void GPIO_Config(void) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);
}

void MPU6050_Init(void) {
    uint8_t whoAmI;
    
    // Reset the device
    I2C_WriteByte(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x80);
    Delay_Ms(100);  // Wait for reset to complete
    
    // Wake up the device
    I2C_WriteByte(MPU6050_ADDR, MPU6050_PWR_MGMT_1, 0x00);
    Delay_Ms(100);
    
    // Verify device ID
    whoAmI = I2C_ReadByte(MPU6050_ADDR, MPU6050_WHO_AM_I);
    if (whoAmI != 0x68) {
        printf("MPU6050 not found! WHO_AM_I = 0x%02X\r\n", whoAmI);
        while(1);  // Stop if device not found
    }
    
    // Configure accelerometer for ±2g range
    I2C_WriteByte(MPU6050_ADDR, MPU6050_ACCEL_CONFIG, 0x00);
    Delay_Ms(10);
    
    printf("MPU6050 initialized successfully\r\n");
}

void MPU6050_Read_Accel(void) {
    uint8_t buf[6];
    for(int i = 0; i < 6; i++) {
        buf[i] = I2C_ReadByte(MPU6050_ADDR, MPU6050_ACCEL_XOUT_H + i);
    }
    accelX = (buf[0] << 8) | buf[1];
    accelY = (buf[2] << 8) | buf[3];
    accelZ = (buf[4] << 8) | buf[5];
}

void Check_Tilt(void) {
    if (abs(accelX) > TILT_THRESHOLD || abs(accelY) > TILT_THRESHOLD) {
        if (!tiltDetected) {
            printf("TILT WARNING!\r\n");
            tiltDetected = 1;
        }
        GPIO_WriteBit(GPIOD, GPIO_Pin_6, SET);  // LED ON
    } else {
        tiltDetected = 0;
        GPIO_WriteBit(GPIOD, GPIO_Pin_6, RESET);  // LED OFF
    }
}

void IIC_Init(u32 bound, u16 address) {
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    I2C_InitTypeDef I2C_InitTSturcture = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    I2C_InitTSturcture.I2C_ClockSpeed = bound;
    I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
    I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitTSturcture.I2C_OwnAddress1 = address;
    I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
    I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_Init(I2C1, &I2C_InitTSturcture);

    I2C_Cmd(I2C1, ENABLE);
}

void I2C_WriteByte(uint8_t addr, uint8_t reg, uint8_t data) {
    uint16_t timeout = I2C_TIMEOUT;
    
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && --timeout);
    if (!timeout) {
        printf("I2C busy timeout\r\n");
        return;
    }
    
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && --timeout);
    if (!timeout) {
        printf("I2C start timeout\r\n");
        return;
    }
    
    I2C_Send7bitAddress(I2C1, addr << 1, I2C_Direction_Transmitter);
    timeout = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && --timeout);
    if (!timeout) {
        printf("I2C address timeout\r\n");
        return;
    }
    
    I2C_SendData(I2C1, reg);
    timeout = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && --timeout);
    if (!timeout) {
        printf("I2C register timeout\r\n");
        return;
    }
    
    I2C_SendData(I2C1, data);
    timeout = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && --timeout);
    if (!timeout) {
        printf("I2C data timeout\r\n");
        return;
    }
    
    I2C_GenerateSTOP(I2C1, ENABLE);
}

uint8_t I2C_ReadByte(uint8_t addr, uint8_t reg) {
    uint8_t data;
    uint16_t timeout = I2C_TIMEOUT;
    
    while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY) && --timeout);
    if (!timeout) {
        printf("I2C busy timeout\r\n");
        return 0;
    }
    
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && --timeout);
    if (!timeout) {
        printf("I2C start timeout\r\n");
        return 0;
    }
    
    I2C_Send7bitAddress(I2C1, addr << 1, I2C_Direction_Transmitter);
    timeout = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED) && --timeout);
    if (!timeout) {
        printf("I2C address timeout\r\n");
        return 0;
    }
    
    I2C_SendData(I2C1, reg);
    timeout = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED) && --timeout);
    if (!timeout) {
        printf("I2C register timeout\r\n");
        return 0;
    }
    
    I2C_GenerateSTART(I2C1, ENABLE);
    timeout = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT) && --timeout);
    if (!timeout) {
        printf("I2C restart timeout\r\n");
        return 0;
    }
    
    I2C_Send7bitAddress(I2C1, addr << 1, I2C_Direction_Receiver);
    timeout = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED) && --timeout);
    if (!timeout) {
        printf("I2C receive mode timeout\r\n");
        return 0;
    }
    
    I2C_AcknowledgeConfig(I2C1, DISABLE);
    I2C_GenerateSTOP(I2C1, ENABLE);
    
    timeout = I2C_TIMEOUT;
    while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED) && --timeout);
    if (!timeout) {
        printf("I2C receive timeout\r\n");
        return 0;
    }
    
    data = I2C_ReceiveData(I2C1);
    I2C_AcknowledgeConfig(I2C1, ENABLE);
    return data;
}

int main(void) {
    USART_Printf_Init(115200);
    printf("\r\nMPU6050 Tilt Detection Starting...\r\n");
    
    Delay_Init();
    GPIO_Config();
    IIC_Init(400000, 0x00);  // Increased to 400kHz
    MPU6050_Init();
    
    while(1) {
        MPU6050_Read_Accel();
        Check_Tilt();
        Delay_Ms(100);
    }
}

void NMI_Handler(void) {}
void HardFault_Handler(void) {
    while (1) {}
}
