
set(fi_SRCS tif_aux.c
    tif_close.c
    tif_codec.c
    tif_color.c
    tif_compress.c
    tif_dir.c
    tif_dirinfo.c
    tif_dirread.c
    tif_dirwrite.c
    tif_dumpmode.c
    tif_error.c
    tif_extension.c
    tif_fax3.c
    tif_fax3sm.c
    tif_flush.c
    tif_getimage.c
    tif_jbig.c
    tif_jpeg.c
    tif_luv.c
    tif_lzw.c
    tif_next.c
    tif_ojpeg.c
    tif_open.c
    tif_packbits.c
    tif_pixarlog.c
    tif_predict.c
    tif_print.c
    tif_read.c
    tif_strip.c
    tif_swab.c
    tif_thunder.c
    tif_tile.c
    tif_version.c
    tif_warning.c
    tif_write.c
    tif_zip.c)

set(fi_HDRS t4.h
    tif_config.h
    tif_config.vc.h
    tif_config.wince.h
    tif_dir.h
    tif_fax3.h
    tif_predict.h
    tiff.h
    tiffconf.vc.h
    tiffconf.wince.h
    tiffio.h
    tiffiop.h
    tiffvers.h
    uvcode.h
    )

if ( WIN32 )
     set(fi_PLAT tif_win32.c)
endif ()

if ( UNIX )
    SET(fi_PLAT tif_unix.c)
endif ()

if ( APPLE )
    SET(fi_PLAT tif_apple.c)
endif ()


include_directories(${FI_INCLUDE} )
add_library(LibTIFF ${fi_SRCS} ${fi_HDRS} ${fi_PLAT})
