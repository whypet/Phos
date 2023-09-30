# Phos
Phosphor, called Phos for short, is a work in progress hobbyist operating system.

## Design

It should be a minimal x86_64 megalithic kernel fully written in C and Assembly. \
Strings will be UTF-32 internally. \
Phos has no compability with other compilers or architectures and there are currently no plans to add it.

## Goals

### Bootloader

- [x] Basic string and memory functions (we aren't using the EDK II build system, this is bare bones)
- [x] PE loader with relocation

### Kernel

- [ ] GDT (make new one after UEFI boot stage)
- [ ] APIC interrupt handling (exceptions, keyboard...)
- [ ] Kernel debugging (possibly own debugger)
- [ ] Paging
- [ ] Another PE loader
- [ ] Driver interface
- [ ] FAT32 driver
- [ ] Command-line interface
- [ ] Scheduler
- [ ] Basic window manager & user interface
- [ ] ACPI control
- [ ] SoundBlaster 16 driver (maybe?)
