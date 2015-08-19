
SET(SRC_LIST
	dl_iso8583.c
	dl_iso8583_defs_1987.c
	dl_iso8583_defs_dynamic.c
	dl_mem.c
	dl_str.c
	dl_timer.c
	dl_iso8583_common.c
	dl_iso8583_defs_1993.c
	dl_iso8583_fields.c
        dl_output.c
	dl_time.c
	)	

ADD_LIBRARY(iso8583 ${SRC_LIST})

