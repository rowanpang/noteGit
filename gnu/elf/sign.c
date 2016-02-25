

typedef unsigned int Elf64_Word;

typedef struct elf64_note {
	Elf64_Word n_namesz;
	Elf64_Word n_descsz;
	Elf64_Word n_type;
} Elf64_Nhdr;

//"使用.note.**,section type 变为NOTE“
static const struct
{
	struct elf64_note _nhdr;
	unsigned char _name[sizeof("module.sig")] __attribute__ ((aligned(sizeof(Elf64_Word))));
	unsigned char _desc[100]  __attribute__ ((aligned(sizeof(Elf64_Word))));
} _note_3 __attribute__((__used__)) __attribute__ ((section(".note.module.sig"), aligned(sizeof(Elf64_Word)), unused)) =

{
	{ 
		sizeof("module.sig"), 
		100,
		100,
	},
	"module.sig",
	"my content"
};						       
