<h1 align="center">Phos</h1>

Phosphor, called Phos for short, is a work in progress hobbyist operating system.

## Design

- It should be a minimal x86_64 megalithic kernel fully written in C and Assembly.
	- As such, all drivers and applications will run under kernel mode.
- Phos is not written with security, speed or size in mind. Many parts of it are written in such ways that are more comfortable and simpler as this is mainly a learning project.
- Phos is primarily inspired from Windows NT while also taking some ideas from Unix.
	- The coding style and APIs are very similar to NT.
	- Only PE+ binaries are supported.
	- Binaries don't have file extensions.
- Only UTF-32 strings will be used internally. This was chosen because it's easier to work with in the kernel, but UTF-8 encoding and decoding will be added later.

## Building

Phos uses the CMake build system with the Clang compiler. \
It's preferable to work with Phos inside Visual Studio Code where it can also be built with the CMake Tools extensions.

## Limitations

Phos has no compability with other compilers or platforms and there are currently no plans to add it. This is my first operating system development project, so it'll be very limited.

As of now, it's also only being tested under Clang and QEMU, and due to some quirks in the code such as both GCC and MSVC attributes being used it can't be built under any other compiler.

## Features

A list of implemented and planned features of the operating system.

### Phosboot &mdash; an UEFI bootloader

We aren't using the EDK II build system so this is bare bones. \
However, since this is UEFI, there's still a lot that is done for us that we don't have to deal with (e.g: setting up long mode, graphics output, filesystem support).

- [x] Basic string and memory functions
- [x] PE loader with relocation

### Phoskrnl &mdash; the operating system kernel

- [ ] GDT (make new one after UEFI boot stage)
- [ ] APIC interrupt handling (exceptions, keyboard...)
- [ ] Paging
- [ ] Command-line interface
- [ ] Internal FAT32 driver
- [ ] Another PE+ loader
- [ ] Driver interface
- [ ] Scheduler
- [ ] Basic window manager & user interface
- [ ] ACPI control
- [ ] SoundBlaster 16 driver (maybe?)

### Phosdb &mdash; a GUI debugger written for the kernel

*Phosdb only runs under Windows due to Direct3D and WinAPI usage.*

- [x] Initialize D3D11 and ImGui
- [ ] Read from serial ports with QEMU
- [ ] Memory view
- [ ] DWARF debug info parsing
- [ ] Source code view & walking the stack
- [ ] Expressions
- [ ] Watch
