# Building a Modern Computer from First Principles

The materials are aimed at students, instructors, and self-learners. Everything is free and open-source; as long as you operate in a non-profit educational setting, you are welcome to modify and use our materials as you see fit.

The materials also support two courses that we now teach in Coursera:

[Nand2Tetris Part I] (hardware, projects/chapters 1-6) is offered as an on-demand course that learners take at their own pace. Here is a two-minute video promo of this course.

[Nand2Tetris Part II] (software, projects/chapters 7-12): is also offered on Coursera, in the same format.

# Software

*The Nand2Tetris Software Suite* contains all the tools and files necessary to complete all the projects described in this site, and in the book *The Elements of Computing Systems*. Once you download the nand2tetris software suite to your PC, there is no need to download anything else throughout your Nand to Tetris learning experience.

The software runs as is on Windows, Unix, and Mac OS.

The software can be used freely under the terms of the [GNU GPL (General Public License)](https://www.gnu.org/copyleft/gpl.html).

The software is open source. If you wish to inspect, modify, or extend the software, see the bottom of this page.

### About the Software

The *Nand2tetris Software Suite* consists of two directories: `projects`, and `tools`.

The `projects` directory is divided into 14 project directories named `00`, `01`, ..., `13` (of which project `00` is a warm-up and project `13` is open-ended). These directories contain files that you have to modify and complete as you work on completing the various nand2tetris projects.

The `tools` directory contains the nand2tetris software tools. There is no need to inspect or use any of these tools, unless you are advised to do so in the project on which you are presently working. In other words, everything is project-driven. The remainder of this section should be used as reference; there is no need to read what follows until you will be asked to use a particular software tool.

**The .bat and .sh files** are batch and script files, used to invoke the nand2tetris software tools. These files are explained in detail below.

**The bin directory** contains the code of the nand2tetris software tools. It consists of several subdirectories containing Java class files and supporting files.

**The builtInChips and the builtInVMCode directories** contain files that are used by the supplied Hardware Simulator and VM Emulator, respectively.

**The OS directory** contains a compiled version of the Jack operating system.

### Software Tools

The *Nand2tetris Software Suite* features the following software tools:

| Tool | Description |
| :--: | ----------- |
| Hardware Simulator |	Simulates and tests logic gates and chips implemented in the HDL (Hardware Description Language) described in the book. Used in hardware construction projects. 
| CPU Emulator |	Emulates the operation of the Hack computer system. Used to test and run programs written in the Hack machine language, in both its binary and assembly versions.
| VM Emulator |	Emulates the operation of our virtual machine (similar to Java's JVM); used to run and test programs written in the VM language (similar to Java's Bytcode). 
| Assembler	| Translates programs from the Hack assembly language to Hack binary code. The resulting code can be executed directly on the *Computer chip* (in the hardware simulator), or emulated on the supplied *CPU Emulator* (much faster and more convenient).
| Compiler | Translates programs written in Jack (a simple, Java-like object-based language) into VM code. The resulting code can run on the supplied *VM Emulator*. Alternatively, the VM code can be translated further by the supplied *VM translator* into Hack assembly code that can then be executed on the supplied *CPU Emulator*. 
| Operating system | Two OS implementations are supplied: (i) a collection of eight `.vm` class files, written originally in Jack (just like Unix is written in C), and (ii) a faster implementation of all the OS services, embedded in the supplied *VM Emulator*. 
| Text Comparer	| This utility checks if two input text files are identical, up to white space differences. Used in various projects. In Unix use "diff" instead. |

### Source Code

All the nand2tetris software tools are written in Java. If you wish to inspect, modify, or extend some tool, you can [download the source code](http://nand2tetris.org/software/nand2tetris-open-source-2.5.7.zip).
