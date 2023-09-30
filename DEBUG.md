# Debugging

Phos is currently debugged using Visual Studio Code, CMake Tools and CodeLLDB. (GDB currently doesn't work)

When building with CMake, you must use the Debug build type.

## Bootloader

The bootloader and LLDB are launched with the `(lldb) Launch` configuration in VSCode. Upon starting, you will immediately hit a breakpoint. This is because QEMU is waiting for the debugger before starting emulation, so continue.

After that, the UEFI application's image base will be displayed. In the debug console, you need to type `target modules load --file BootX64.efi .text <base>` where `<base>` is the image base seen earlier and the base of the binary's .text section added together. The .text section is usually located at 0x1000.

Now you need to interrupt the process with LLDB and if everything went correctly it should break in the source code, specifically at the `while (Pause)` loop in `Main.c` where it's waiting for the debugger. Type `expr Pause = 0` in the debug console to end the loop and then you can debug the bootloader however you want.

## Kernel

The kernel will have a separate debugger (Phosdb) written using serial port communication. It is not written yet.