**Exploring Compilation Units in C Report**

# Assignment 1

The program is divided into three files: main.c, math_utils.c, and math_utils.h.

# Assignment 2

The source files are compiled separately using gcc -c. This creates main.o and math_utils.o. These are relocatable object files, so they contain machine code and symbol information, but references between separately compiled files are not fully resolved yet.

The two object files are then linked using gcc main.o math_utils.o -o square_prog. The linker combines the object files and resolves the reference to square() from main.o using the definition in math_utils.o.

# Assignment 3 – nm Analysis

nm shows the symbols stored in each file. T means that a symbol is defined in the text section, and U means that the symbol is undefined in that file.

**nm main.o**

0000000000000000 T main  
U printf  
U square

main is defined in main.o. printf and square are undefined because their definitions are not inside this object file. square will be provided by math_utils.o, while printf is provided through the C library.

**nm math_utils.o**

0000000000000000 T square

square is defined in math_utils.o, so it appears with T. This is the function that main.o references.

**nm square_prog**

0000000000001149 T main  
0000000000001188 T square  
U printf@GLIBC_2.2.5

After linking, main and square are both defined in the executable. printf is dynamically linked to the C library, so it still appears as an undefined external symbol.

# Assignment 4 – objdump Analysis

objdump -d shows the generated machine instructions. In main.o, the calls to square() and printf() are present, but their final addresses are not yet resolved. In math_utils.o, the disassembly contains the instructions for square().

**objdump -d main.o**

0000000000000000 \<main\>:  
0: 55 push %rbp  
1: 48 89 e5 mov %rsp,%rbp  
...  
1a: e8 00 00 00 00 call 1f \<main+0x1f\>  
2c: e8 00 00 00 00 call 31 \<main+0x31\>  
31: b8 00 00 00 00 mov \$0x0,%eax  
36: 5d pop %rbp  
37: c3 ret

The unresolved call instructions are consistent with the undefined symbols shown by nm. Their final targets are supplied during linking.

**objdump -d math_utils.o**

0000000000000000 \<square\>:  
0: 55 push %rbp  
1: 48 89 e5 mov %rsp,%rbp  
4: 89 7d fc mov %edi,-0x4(%rbp)  
7: 8b 45 fc mov -0x4(%rbp),%eax  
a: 0f af c0 imul %eax,%eax  
d: 5d pop %rbp  
e: c3 ret

The instructions take the integer argument, multiply it by itself, and return the result. This is the machine-code form of square().

**objdump -d square_prog**

0000000000001149 \<main\>:  
...  
117d: e8 06 00 00 00 call 1188 \<square\>  
...  
0000000000001188 \<square\>:  
...  
1194: 0f af c0 imul %eax,%eax  
...

In the final executable, main() calls square() at its resolved address, 0x1188. The executable also contains startup code and dynamic-linking structures needed to run the program.

# Assignment 5 – readelf ELF Header

readelf -h shows the basic ELF structure. The object files are relocatable files, while the final program is an executable.

**readelf -h main.o**

Class: ELF64  
Type: REL (Relocatable file)  
Machine: Advanced Micro Devices X86-64  
Start of program headers: 0 (bytes into file)  
Number of program headers: 0

main.o is an ELF64 relocatable file. It has no program headers because it is not a runnable executable yet.

**readelf -h math_utils.o**

Class: ELF64  
Type: REL (Relocatable file)  
Machine: Advanced Micro Devices X86-64  
Start of program headers: 0 (bytes into file)  
Number of program headers: 0

math_utils.o is also an ELF64 relocatable file and is intended to be combined with other object files by the linker.

**readelf -h square_prog**

Class: ELF64  
Type: DYN (Position-Independent Executable file)  
Machine: Advanced Micro Devices X86-64  
Entry point address: 0x1060  
Number of program headers: 13

square_prog is an ELF64 position-independent executable on this system. It has an entry point and program headers because it is a runnable program.

# Assignment 6 – ELF Section Analysis

readelf -S shows the sections stored in each ELF file. The object files have the sections needed for separate compilation, while the final executable contains additional sections for execution and dynamic linking.

**main.o**

.text  
.rela.text  
.data  
.bss  
.rodata  
.comment  
.note.GNU-stack  
.note.gnu.property  
.eh_frame  
.rela.eh_frame  
.symtab  
.strtab  
.shstrtab

main.o contains code in .text, read-only data in .rodata, and relocation information in .rela.text. The relocation information is needed because some references are not resolved until linking.

**math_utils.o**

.text  
.data  
.bss  
.comment  
.note.GNU-stack  
.note.gnu.property  
.eh_frame  
.rela.eh_frame  
.symtab  
.strtab  
.shstrtab

math_utils.o has fewer sections because it only contains the square() function and does not have the same unresolved external reference as main.o.

**square_prog**

.interp  
.note.gnu.property  
.note.gnu.build-id  
.note.ABI-tag  
.gnu.hash  
.dynsym  
.dynstr  
.gnu.version  
.gnu.version_r  
.rela.dyn  
.rela.plt  
.init  
.plt  
.plt.got  
.plt.sec  
.text  
.fini  
.rodata  
.eh_frame_hdr  
.eh_frame  
.init_array  
.fini_array  
.dynamic  
.got  
.data  
.bss  
.comment  
.symtab  
.strtab  
.shstrtab

The executable has many additional sections, including dynamic symbol information, the executable code, read-only data, and initialization and finalization sections. These are part of the complete executable structure.

1. gcc -E main.c -o main.i
   Preprocessing — expands headers and macros.
2. gcc -S main.i -o main.s
   Compilation — generates assembly code.
3. gcc -c main.s -o main.o
   Assembly — generates an object file (machine code).
4. gcc main.o math_utils.o -o square_prog
   Linking — combines object files into an executable.

**GitHub Link:** [*https://github.com/inesanshanyan/OperatingSystemsClass/tree/main/homeworks/hw4*](https://github.com/inesanshanyan/OperatingSystemsClass/tree/main/homeworks/hw4)
