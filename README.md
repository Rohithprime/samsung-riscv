# Documentation for the Samsung RISC-V Workshop

Welcome to the documentation for the Samsung Semiconductor RISC-V Workshop! Using the VSDSquadron Mini RISC-V development board, this session investigates the open-source RISC-V architecture. As a pioneer in technology, Samsung is dedicated to developing RISC-V technology and encouraging creativity via cooperation and education.
-------------------------------------------------

<details>
<summary><b>Task 1:</b> The assignment is to watch lab videos that are based on C and RISCV and then use the gcc and riscv compilers to compile the C code</summary>

### C Language based LAB
We have to follow the given steps to compile any **.c** file in our machine:  
1. Open the bash terminal and locate to the directory where you want to create your file. Then run the following command:

	```
	gedit sum.c
	```  
2. This will open the editor and allows you to write into the file that you have created. You have to write the C code of printing the sum of n numbers. Once you are done with your code, press ```Ctrl + S``` to save your file, and then press ```Ctrl + W``` to close the editor.   
3. To the C code on your terminal, run the following command:

	```
	gcc sum.c
	./a.out
	```
### C Code compiled on gcc Compiler:
 <p align="center">
  <img width="800" height="500" src="/Task 1/C Code compiled on gcc Compiler.png">
</p>

### Compiled C output:
 <p align="center">
  <img width="800" height="500" src="/Task 1/compiled C output.png">
</p>

### RISCV based LAB
We have to do the same compilation of our code but this time using RISCV gcc compiler. Follow the given steps:  
1. Open the terminal and run the given command:  

	```
	cat sum.c
	```
### Cat Command:
 <p align="center">
  <img width="800" height="500" src="/Task 1/cat Command.png">
</p>

2. Using the **cat** command, the entire C code will be displayed on the terminal. Now run the following command to compile the code in riscv64 gcc compiler:  

	```
	riscv64-unknown-elf-gcc -O1 -mabi=lp64 -march=rv64i -o sum.o sum.c
	```
3. Open a new terminal and run the given command:    

	```
	riscv64-unknown-elf-objdump -d sum.o
	```
### Objdump using -O1 format:
 <p align="center">
  <img width="800" height="500" src="/Task 1/Objdump using -O1 format.png">
</p>

4. Open the previous tab and run the following command to compile the code in riscv64 gcc compiler:  

	```
	riscv64-unknown-elf-gcc -Ofast -mabi=lp64 -march=rv64i -o sum.o sum.c
	```

5. Open a new terminal and run the given command:    

	```
	riscv64-unknown-elf-objdump -d sum.o
	```
### Objdump using -Ofast format:
 <p align="center">
  <img width="800" height="500" src="/Task 1/Objdump using -Ofast format.png">
</p>
</details>


-------------------------------------------------

<details>
<summary><b>Task 2:</b> Using Spike Simulation and Interactive Debugging Mode to Debugg the C code during Spike</summary> 

### What is SPIKE in RISCV?
> * A RISC-V ISA is a simulator, enabling the testing and analysis of RISC-V programs without the need for actual hardware.  
> * Spike is a free, open-source C++ simulator for the RISC-V ISA that models a RISC-V core and cache system. It can be used to run programs and a Linux kernel, and can be a starting point for running software on a RISC-V target.    
  
### What is pk (Proxy Kernel)?  
> * The RISC-V Proxy Kernel, pk , is a lightweight application execution environment that can host statically-linked RISC-V ELF binaries.  
> * A Proxy Kernel in the RISC-V ecosystem simplifies the interaction between complex hardware and the software running on it, making it easier to manage, test, and develop software and hardware projects.  

### Testing the SPIKE Simulator  
The target is to run the ```sum.c``` code using both ```gcc compiler``` and ```riscv compiler```, and both of the compiler must display the same output on the terminal. So to compile the code using **gcc compiler**, use the following command:  
```
gcc sum.c  
./a.out
```
And to compile the code using **riscv compiler**, use the following command:  
```
spike pk sum.o
```  
#### Spike Simulation:
 <p align="center">
  <img width="800" height="500" src="/Task 2/Spike Simulation.png">
</p>

#### Following are the snapshots of RISCV Objdump with **-O1** and **-Ofast** options  
  
#### Objdump in -O1:

```
riscv64-unknown-elf-gcc -O1 -mabi=lp64 -march=rv64i -o sum.o sum.c
```

 <p align="center">
  <img width="800" height="500" src="/Task 2/Objdump in -O1.png">
</p>
  
#### Objdump in -Ofast:

```
riscv64-unknown-elf-gcc -Ofast -mabi=lp64 -march=rv64i -o sum.o sum.c
```
 
 <p align="center">
  <img width="800" height="500" src="/Task 2/Objdump in -Ofast.png">
</p>


#### Debugging the Assembly Language Program of  ```sum.c```  
* Open the **Objdump** of code by using the following command  
```
riscv64-unknown-elf-objdump -d sum.o | less  
```
* Open the debugger in another terminal by using the following command  
```
spike -d pk sum.o
```
* The debugger will be opened in the terminal. Now, debugging operations can be performed as shown in the following snapshot.

#### Debugging:
 <p align="center">
  <img width="800" height="500" src="/Task 2/Debugging.png">
</p>
</details>

----------------------------------------

<details>
<summary><b>Task 3:</b> Identifying RISC-V Instruction Types</summary>

## INSTRUCTIONS FORMAT IN RISC-V
The instruction format of a processor dictates how machine language instructions are structured and organized for the processor to execute. Each instruction is composed of a series of 0s and 1s, with each segment containing information about the location and operation of data.  
There are six primary instruction formats in RISC-V:

1. R-format
2. I-format
3. S-format
4. B-format
5. U-format
6. J-format

#### RISCV Instruction Types

 <p align="center">
  <img src="/Task 3/Instruction_Types.png">
</p>

#### 1. R-type Instruction
In RV32, each instruction is 32 bits in size. R-type instructions perform operations on registers (not memory) and are used for various arithmetic and logical operations. The 32-bit instruction is divided into six fields:

 <p align="center">
  <img src="/Task 3/Instruction_R_Type.png">
</p>

- **opcode** (7 bits): Specifies the type of instruction.
- **rd** (5 bits): The destination register where the result of the operation is stored.
- **func3** (3 bits): Specifies the type of operation performed.
- **rs1, rs2** (5 bits each): Source registers used in the operation.
- **func7** (7 bits): Further specifies the operation.

#### 2. I-type Instruction
I-type instructions involve operations that use both registers and an immediate value (not memory). These instructions are used for immediate and load operations. The instruction format is as follows:

 <p align="center">
  <img src="/Task 3/Instruction_I_Type.png">
</p>

- **opcode** (7 bits): Specifies the type of instruction.
- **rd** (5 bits): The destination register for the result.
- **func3** (3 bits): Specifies the type of operation.
- **rs1** (5 bits): Source register.
- **imm[11:0]** (12 bits): A 12-bit signed immediate value used in the operation.

#### 3. S-type Instruction
S-type instructions are used for store operations where data is stored from a register to memory. The 32-bit instruction is divided as follows:

 <p align="center">
  <img src="/Task 3/Instruction_S_Type.png">
</p>

- **opcode** (7 bits): Specifies the type of instruction.
- **imm[11:5]** (7 bits) and **imm[4:0]** (5 bits): The 12-bit immediate value is split across two fields, specifying the store offset.
- **rs1** (5 bits): The register containing the data to store.
- **rs2** (5 bits): The register containing the address where data should be stored.
- **func3** (3 bits): Specifies the type of store (byte, half-word, or word).

#### 4. B-type Instruction
B-type instructions are used for conditional branching based on comparisons. The 32-bit instruction format is as follows:

 <p align="center">
  <img src="/Task 3/Instruction_B_Type.png">
</p>

- **opcode** (7 bits): Specifies the type of instruction.
- **imm[12]** (1 bit), **imm[10:5]** (6 bits), **imm[4:1]** (4 bits), and **imm[11]** (1 bit): These bits form the 12-bit signed immediate used for the branch offset.
- **rs1, rs2** (5 bits each): Source registers involved in the comparison.
- **func3** (3 bits): Defines the condition used for branching.

#### 5. U-type Instruction
U-type instructions are used to transfer an immediate value into the destination register. The format is simple and involves only two instructions: `LUI` and `AUIPC`.

 <p align="center">
  <img src="/Task 3/Instruction_U_Type.png">
</p>

- **opcode** (7 bits): Specifies the type of instruction.
- **rd** (5 bits): The destination register for the immediate value.
- **imm[19:0]** (20 bits): The 20-bit immediate value that is transferred to the destination register.

For example, the instruction `lui x15, 0x13579` would load the value `0x13579000` into the upper 20 bits of register `x15`.

#### 6. J-type Instruction
J-type instructions are used for jump operations. These instructions are often used for loops and branching to a specified memory location. The format is as follows:

 <p align="center">
  <img src="/Task 3/Instruction_J_Type.png">
</p>

- **opcode** (7 bits): Specifies the type of instruction.
- **imm[20]** (1 bit), **imm[10:1]** (10 bits), **imm[11]** (1 bit), and **imm[19:12]** (8 bits): These bits form the 20-bit signed immediate for the jump address.
- **rd** (5 bits): The destination register (used for return addresses).

# Example Application with its Instructions
<details>
<summary>Given below is the C code for sum of N numbers</summary>
	
```c
#include<stdio.h>

void main()
{
	int sum = 0,n = 10,i = 1;
	while(i != n+1)
	{
		sum += i;
		i++;
	}
	printf("sum of first %d numbers is %d\n",n,sum);    

}
```
</details>

# Instructions with explaination
<details>
<summary>Now, let's analyse each instruction one by one</summary>
	
# RISC-V Instructions Explanation

This document provides a detailed explanation of the given RISC-V assembly instructions, including their types, functionality, and corresponding 32-bit representations.

### 1. `add a5, a4, a5`
- **Type**: R-type  
- **Description**: Adds the values in `a4` and `a5`, and stores the result in `a5`.  
- **Fields**:  
  - **opcode**: 0110011  
  - **rd**: a5 = 10101  
  - **rs1**: a4 = 10100  
  - **rs2**: a5 = 10101  
  - **func3**: 000  
  - **func7**: 0000000  
- **32-bit Instruction**: `0000000_10101_10100_000_10101_0110011`

---

### 2. `addi a2, a3, 4`
- **Type**: I-type  
- **Description**: Adds the immediate value `4` to the value in `a3`, and stores the result in `a2`.  
- **Fields**:  
  - **opcode**: 0010011  
  - **rd**: a2 = 10010  
  - **rs1**: a3 = 10011  
  - **imm**: 0000000000000100  
  - **func3**: 000  
- **32-bit Instruction**: `0000000000000100_10011_000_10010_0010011`

---

### 3. `sub a1, a1, a2`
- **Type**: R-type  
- **Description**: Subtracts the value in `a2` from the value in `a1`, and stores the result in `a1`.  
- **Fields**:  
  - **opcode**: 0110011  
  - **rd**: a1 = 10001  
  - **rs1**: a1 = 10001  
  - **rs2**: a2 = 10010  
  - **func3**: 000  
  - **func7**: 0100000  
- **32-bit Instruction**: `0100000_10010_10001_000_10001_0110011`

---

### 4. `and a5, a5, a2`
- **Type**: R-type  
- **Description**: Performs a bitwise AND operation between the values in `a5` and `a2`, and stores the result in `a5`.  
- **Fields**:  
  - **opcode**: 0110011  
  - **rd**: a5 = 10101  
  - **rs1**: a5 = 10101  
  - **rs2**: a2 = 10010  
  - **func3**: 111  
  - **func7**: 0000000  
- **32-bit Instruction**: `0000000_10010_10101_111_10101_0110011`

---

### 5. `or a5, a5, a6`
- **Type**: R-type  
- **Description**: Performs a bitwise OR operation between the values in `a5` and `a6`, and stores the result in `a5`.  
- **Fields**:  
  - **opcode**: 0110011  
  - **rd**: a5 = 10101  
  - **rs1**: a5 = 10101  
  - **rs2**: a6 = 10110  
  - **func3**: 110  
  - **func7**: 0000000  
- **32-bit Instruction**: `0000000_10110_10101_110_10101_0110011`

---

### 6. `xor a4, a7, a4`
- **Type**: R-type  
- **Description**: Performs a bitwise XOR operation between the values in `a7` and `a4`, and stores the result in `a4`.  
- **Fields**:  
  - **opcode**: 0110011  
  - **rd**: a4 = 10100  
  - **rs1**: a7 = 10111  
  - **rs2**: a4 = 10100  
  - **func3**: 100  
  - **func7**: 0000000  
- **32-bit Instruction**: `0000000_10100_10111_100_10100_0110011`

---

### 7. `lw a2, 20(sp)`
- **Type**: I-type  
- **Description**: Loads a word from the memory address obtained by adding `20` to the value in `sp`, and stores it in `a2`.  
- **Fields**:  
  - **opcode**: 0000011  
  - **rd**: a2 = 10010  
  - **rs1**: sp = 00010  
  - **imm**: 0000000000010100  
  - **func3**: 010  
- **32-bit Instruction**: `0000000000010100_00010_010_10010_0000011`

---

### 8. `sw a5, 28(sp)`
- **Type**: S-type  
- **Description**: Stores the value in `a5` at the memory address obtained by adding `28` to the value in `sp`.  
- **Fields**:  
  - **opcode**: 0100011  
  - **rs1**: sp = 00010  
  - **rs2**: a5 = 10101  
  - **imm**: 0000000000011100  
  - **func3**: 010  
- **32-bit Instruction**: `0000000_10101_00010_010_11100_0100011`

---

### 9. `beq s2, a2, 21978`
- **Type**: B-type  
- **Description**: Branches to the address offset `21978` if `s2` equals `a2`.  
- **Fields**:  
  - **opcode**: 1100011  
  - **rs1**: s2 = 10110  
  - **rs2**: a2 = 10010  
  - **imm**: 000000010101101110  
  - **func3**: 000  
- **32-bit Instruction**: `0000000_10010_10110_000_1010111010_1100011`

---

### 10. `bne a2, a5, 2174c`
- **Type**: B-type  
- **Description**: Branches to the address offset `2174c` if `a2` does not equal `a5`.  
- **Fields**:  
  - **opcode**: 1100011  
  - **rs1**: a2 = 10010  
  - **rs2**: a5 = 10101  
  - **imm**: 000000010001110100  
  - **func3**: 001  
- **32-bit Instruction**: `0000000_10101_10010_001_1001110100_1100011`

---

### 11. `sll a7, a7, a4`
- **Type**: R-type  
- **Description**: Performs a logical left shift of `a7` by the number of positions specified in the lower bits of `a4`, and stores the result in `a7`.  
- **Fields**:  
  - **opcode**: 0110011  
  - **rd**: a7 = 10111  
  - **rs1**: a7 = 10111  
  - **rs2**: a4 = 10100  
  - **func3**: 001  
  - **func7**: 0000000  
- **32-bit Instruction**: `0000000_10100_10111_001_10111_0110011`

---

### 12. `lui a3, 0x80`
- **Type**: U-type  
- **Description**: Loads the value `0x80` into the upper 20 bits of `a3`.  
- **Fields**:  
  - **opcode**: 0110111  
  - **rd**: a3 = 10011  
  - **imm**: 000000000000100000  
- **32-bit Instruction**: `000000000000100000_10011_0110111`

---

### 13. `jal ra, 247e4`
- **Type**: J-type  
- **Description**: Jumps to the address offset `247e4` and stores the return address in `ra`.  
- **Fields**:  
  - **opcode**: 1101111  
  - **rd**: ra = 00001  
  - **imm**: 0010010001111110100  
- **32-bit Instruction**: `0010010001111110100_00001_1101111`

---

### 14. `jalr a5`
- **Type**: I-type  
- **Description**: Jumps to the address specified in `a5` and stores the return address in `ra`.  
- **Fields**:  
  - **opcode**: 1100111  
  - **rd**: ra = 00001  
  - **rs1**: a5 = 10101  
  - **imm**: 0000000000000000  
  - **func3**: 000  
- **32-bit Instruction**: `0000000000000000_10101_000_00001_1100111`

---

### 15. `auipc a5, 0x6`
- **Type**: U-type  
- **Description**: Adds the immediate value `0x6` shifted left 12 bits to the current PC and stores the result in `a5`.  
- **Fields**:  
  - **opcode**: 0010111  
  - **rd**: a5 = 10101  
  - **imm**: 000000000000000110  
- **32-bit Instruction**: `000000000000000110_10101_0010111`
  
</details>

</details>

## License
This project documentation is licensed under the MIT License.

