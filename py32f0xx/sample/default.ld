ENTRY(_start)

__stack_size = 256;

PROVIDE(_stack_size = __stack_size);

MEMORY {
	FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 16K
	RAM (xrw)  : ORIGIN = 0x20000000, LENGTH = 3K
}

SECTIONS {
	.isr_vector : {
		KEEP(*(SORT_NONE(.isr_vector)))
	} >FLASH AT>FLASH

	.text : {
		. = ALIGN(4);
		*(.rodata .rodata*)
		*(.srodata .srodata.*)
		*(.text .text.*)
		. = ALIGN(4);
	} >FLASH AT>FLASH

	PROVIDE(_etext = .);

	.dalign : {
		. = ALIGN(4);
		PROVIDE(_data_vma = .);
	} >RAM AT>FLASH

	.dlalign : {
		. = ALIGN(4);
		PROVIDE(_data_lma = .);
	} >FLASH AT>FLASH

	.data : {
		. = ALIGN(4);
		*(.data .data.*)
		. = ALIGN(8);
		PROVIDE(__global_pointer$ = . + 0x800);
		*(.sdata .sdata.*)
		*(.sdata2.*)
		. = ALIGN(4);
		PROVIDE(_edata = .);
	} >RAM AT>FLASH

	.bss : {
		. = ALIGN(4);
		PROVIDE(_sbss = .);
		*(.sbss*)
		*(.bss*)
		*(COMMON*)
		. = ALIGN(4);
		PROVIDE(_ebss = .);
	} >RAM AT>FLASH

	PROVIDE(_end = _ebss);
	PROVIDE(end = .);

	.stack ORIGIN(RAM) + LENGTH(RAM) - __stack_size : {
		PROVIDE(_heap_end = .);
		. = ALIGN(4);
		PROVIDE(_susrstack = .);
		. = . + __stack_size;
		PROVIDE(_eusrstack = .);
	} >RAM
}

