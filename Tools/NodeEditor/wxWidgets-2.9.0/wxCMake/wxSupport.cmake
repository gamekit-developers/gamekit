set(wxJPEG
    src/jpeg/jcomapi.c
    src/jpeg/jutils.c
    src/jpeg/jerror.c
    src/jpeg/jmemmgr.c
    src/jpeg/jmemnobs.c
    src/jpeg/jcapimin.c
    src/jpeg/jcapistd.c
    src/jpeg/jctrans.c
    src/jpeg/jcparam.c
    src/jpeg/jdatadst.c
    src/jpeg/jcinit.c
    src/jpeg/jcmaster.c
    src/jpeg/jcmarker.c
    src/jpeg/jcmainct.c
    src/jpeg/jcprepct.c
    src/jpeg/jccoefct.c
    src/jpeg/jccolor.c
    src/jpeg/jcsample.c
    src/jpeg/jchuff.c
    src/jpeg/jcphuff.c
    src/jpeg/jcdctmgr.c
    src/jpeg/jfdctfst.c
    src/jpeg/jfdctflt.c
    src/jpeg/jfdctint.c
    src/jpeg/jdapimin.c
    src/jpeg/jdapistd.c
    src/jpeg/jdtrans.c
    src/jpeg/jdatasrc.c
    src/jpeg/jdmaster.c
    src/jpeg/jdinput.c
    src/jpeg/jdmarker.c
    src/jpeg/jdhuff.c
    src/jpeg/jdphuff.c
    src/jpeg/jdmainct.c
    src/jpeg/jdcoefct.c
    src/jpeg/jdpostct.c
    src/jpeg/jddctmgr.c
    src/jpeg/jidctfst.c
    src/jpeg/jidctflt.c
    src/jpeg/jidctint.c
    src/jpeg/jidctred.c
    src/jpeg/jdsample.c
    src/jpeg/jdcolor.c
    src/jpeg/jquant1.c
    src/jpeg/jquant2.c
    src/jpeg/jdmerge.c
    )
    
set(wxPNG
    src/png/png.c
    src/png/pngerror.c
    src/png/pnggccrd.c
    src/png/pngget.c
    src/png/pngmem.c
    src/png/pngpread.c
    src/png/pngread.c
    src/png/pngrio.c
    src/png/pngrtran.c
    src/png/pngrutil.c
    src/png/pngset.c
    src/png/pngtrans.c
    src/png/pngvcrd.c
    src/png/pngwio.c
    src/png/pngwrite.c
    src/png/pngwtran.c
    src/png/pngwutil.c
)

if (WIN32)
    set(TIFF_PLATFORM_SRC src/tiff/libtiff/tif_win32.c)
endif()

if (APPLE)
    set(TIFF_PLATFORM_SRC src/tiff/libtiff/tif_unix.c)
else()
    set(TIFF_PLATFORM_SRC src/tiff/libtiff/tif_unix.c)
endif()



set(wxTIFF
    ${TIFF_PLATFORM_SRC}
    src/tiff/libtiff/tif_aux.c
    src/tiff/libtiff/tif_close.c
    src/tiff/libtiff/tif_codec.c
    src/tiff/libtiff/tif_color.c
    src/tiff/libtiff/tif_compress.c
    src/tiff/libtiff/tif_dir.c
    src/tiff/libtiff/tif_dirinfo.c
    src/tiff/libtiff/tif_dirread.c
    src/tiff/libtiff/tif_dirwrite.c
    src/tiff/libtiff/tif_dumpmode.c
    src/tiff/libtiff/tif_error.c
    src/tiff/libtiff/tif_extension.c
    src/tiff/libtiff/tif_fax3.c
    src/tiff/libtiff/tif_fax3sm.c
    src/tiff/libtiff/tif_flush.c
    src/tiff/libtiff/tif_getimage.c
    src/tiff/libtiff/tif_jpeg.c
    src/tiff/libtiff/tif_luv.c
    src/tiff/libtiff/tif_lzw.c
    src/tiff/libtiff/tif_next.c
    src/tiff/libtiff/tif_ojpeg.c
    src/tiff/libtiff/tif_open.c
    src/tiff/libtiff/tif_packbits.c
    src/tiff/libtiff/tif_pixarlog.c
    src/tiff/libtiff/tif_predict.c
    src/tiff/libtiff/tif_print.c
    src/tiff/libtiff/tif_read.c
    src/tiff/libtiff/tif_strip.c
    src/tiff/libtiff/tif_swab.c
    src/tiff/libtiff/tif_thunder.c
    src/tiff/libtiff/tif_tile.c
    src/tiff/libtiff/tif_version.c
    src/tiff/libtiff/tif_warning.c
    src/tiff/libtiff/tif_write.c
    src/tiff/libtiff/tif_zip.c
)


set(wxZLIB
    src/zlib/adler32.c
    src/zlib/compress.c
    src/zlib/crc32.c
    src/zlib/gzio.c
    src/zlib/uncompr.c
    src/zlib/deflate.c
    src/zlib/trees.c
    src/zlib/zutil.c
    src/zlib/inflate.c
    src/zlib/infback.c
    src/zlib/inftrees.c
    src/zlib/inffast.c
)

set(wxEXPAT
    src/expat/lib/xmlparse.c
    src/expat/lib/xmlrole.c
    src/expat/lib/xmltok.c
)
