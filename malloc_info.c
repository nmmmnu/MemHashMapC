#include <malloc.h>
#include <stdio.h>

void display_mallinfo(){
	struct mallinfo m  = mallinfo();

	const char *format = "%-30s %-10s = %10d\n";

	printf(format, "Total non-mmapped bytes"	,	"arena"		,	m.arena		);
	printf(format, "# of free chunks" 		,	"ordblks"	,	m.ordblks	);
	printf(format, "# of free fastbin blocks"	,	"smblks"	,	m.smblks	);
	printf(format, "# of mapped regions"		,	"hblks"		,	m.hblks		);
	printf(format, "Bytes in mapped regions"	,	"hblkhd"	,	m.hblkhd	);
	printf(format, "Max. total allocated space"	,	"usmblks"	,	m.usmblks	);
	printf(format, "Free bytes held in fastbins"	,	"fsmblks"	,	m.fsmblks	);
	printf(format, "Total allocated space"		,	"uordblks"	,	m.uordblks	);
	printf(format, "Total free space"		,	"fordblks"	,	m.fordblks	);
	printf(format, "Topmost releasable block"	,	"keepcost"	,	m.keepcost	);
}

