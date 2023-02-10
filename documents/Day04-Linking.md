
# Linking  

> The process of collecting and combining various pieces of code and data into a single file that can be *loaded*(copied) into memory and executed.  

- **when to perform**: compile time, load time, run time  
- **role**: enable separate compilation  

## Compiler Drivers  

> invokes the language preprocessor, compiler, assembler, and linker, as needed on behalf of the user  

**Summary**:  
source file ```main.cpp``` $\Rightarrow$ 
C preprocessor *cpp* $\Rightarrow$ 
ASCII intermediate file ```main.i``` $\Rightarrow$ C compiler *cc1* $\Rightarrow$ 
ASCII assembly-language file ```main.s``` 
$\Rightarrow$ assembler *as* $\Rightarrow$ 
binary relocatable object file ```main.o``` 
$\Rightarrow$ linker *ld* $\Rightarrow$ 
binary executable object file ```main```  

## Static Linking  

- Two main tasks:  
  1. Sysbol resolution  
     Object files define and reference *sysbols*. The purpose of symbol resolution is to associate each symbol *reference* with exactly one symbol *definition*  
  2. Relocation  
     Compilers and assemblers generate code and data sections that start at address 0.  
     The linker *relocates* these sections by associating a memory location with each symbol definition. and then modifying all of the references to those symbols so that they point to this memory location  

## Object Files  

Three forms:  
  *Relocatable object file* can be loaded at compile time to create an executable object file  
  *Executable object file* can be copied directly into memory and executed  
  *Shared object file* is a special type of relocatable object file that can be loaded into memory and linked dynamically, at either load time or run time  

## Relocatable Object Files  

The format of a typical ELF(Executable and Linkable Format) relocatable object file:  

1. ELF header  
2. ```.text```
3. ```.rodata```
4. ```.data```
5. ```.bss```
6. ```symtab```
7. ```.rel.text```
8. ```.rel.data```
9. ```.line```
10. ```.strtab```
11. Section header table 

## Symbols and Symbol Tables  


