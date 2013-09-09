include_directories( include )
	
set(FI_INCLUDE
	${OGREKIT_FREEIMAGE_INCLUDE}
	${OGREKIT_FREEIMAGE_INCLUDE}/DeprecationManager
	${OGREKIT_FREEIMAGE_INCLUDE}/FreeImage
	${OGREKIT_FREEIMAGE_INCLUDE}/LibJPEG
	${OGREKIT_FREEIMAGE_INCLUDE}/LibMNG
	${OGREKIT_FREEIMAGE_INCLUDE}/LibOpenJPEG
	${OGREKIT_FREEIMAGE_INCLUDE}/LibPNG
	${OGREKIT_FREEIMAGE_INCLUDE}/LibTIFF
	${OGREKIT_FREEIMAGE_INCLUDE}/Metadata
	${OGREKIT_FREEIMAGE_INCLUDE}/ZLib
	${OGREKIT_FREEIMAGE_INCLUDE}/OpenEXR
	${OGREKIT_FREEIMAGE_INCLUDE}/OpenEXR/Half
	${OGREKIT_FREEIMAGE_INCLUDE}/OpenEXR/Iex
	${OGREKIT_FREEIMAGE_INCLUDE}/OpenEXR/IlmImf
	${OGREKIT_FREEIMAGE_INCLUDE}/OpenEXR/IlmThread
	${OGREKIT_FREEIMAGE_INCLUDE}/OpenEXR/Imath
	${OGREKIT_FREEIMAGE_INCLUDE}/LibRawLite
	)


add_definitions(-DFREEIMAGE_LIB -DLIBRAW_NODLL -DOPJ_STATIC)
include_directories(${FI_INCLUDE})

# ---------------------------------------------------------
subdirs(ZLib)

# ---------------------------------------------------------
# FreeImage	SubDir
set(fi_CORE_SRCS
	FreeImage/BitmapAccess.cpp
	FreeImage/CacheFile.cpp
	FreeImage/ColorLookup.cpp
	FreeImage/Conversion.cpp
	FreeImage/Conversion16_555.cpp
	FreeImage/Conversion16_565.cpp
	FreeImage/Conversion24.cpp
	FreeImage/Conversion32.cpp
	FreeImage/Conversion4.cpp
	FreeImage/Conversion8.cpp
	FreeImage/ConversionRGBF.cpp
	FreeImage/ConversionType.cpp
	FreeImage/FreeImage.cpp
	FreeImage/FreeImageIO.cpp
	FreeImage/GetType.cpp
	FreeImage/Halftoning.cpp
	FreeImage/J2KHelper.cpp
	FreeImage/MemoryIO.cpp
	FreeImage/MultiPage.cpp
	FreeImage/NNQuantizer.cpp
	#FreeImage/PSDParser.cpp
	FreeImage/PixelAccess.cpp
	FreeImage/Plugin.cpp
	#FreeImage/PluginBMP.cpp
	#FreeImage/PluginCUT.cpp
	#FreeImage/PluginDDS.cpp
	#FreeImage/PluginEXR.cpp
	#FreeImage/PluginG3.cpp
	#FreeImage/PluginGIF.cpp
	#FreeImage/PluginHDR.cpp
	#FreeImage/PluginICO.cpp
	#FreeImage/PluginIFF.cpp
	#FreeImage/PluginJ2K.cpp
	#FreeImage/PluginJP2.cpp
	FreeImage/PluginJPEG.cpp
	#FreeImage/PluginKOALA.cpp
	#FreeImage/PluginMNG.cpp
	#FreeImage/PluginPCD.cpp
	#FreeImage/PluginPCX.cpp
	#FreeImage/PluginPFM.cpp
	#FreeImage/PluginPICT.cpp
	FreeImage/PluginPNG.cpp
	#FreeImage/PluginPNM.cpp
	#FreeImage/PluginPSD.cpp
	#FreeImage/PluginRAS.cpp
	#FreeImage/PluginRAW.cpp
	#FreeImage/PluginSGI.cpp
	FreeImage/PluginTARGA.cpp
	#FreeImage/PluginTIFF.cpp
	#FreeImage/PluginWBMP.cpp
	#FreeImage/PluginXBM.cpp
	#FreeImage/PluginXPM.cpp
	#FreeImage/TIFFLogLuv.cpp
	#FreeImage/ToneMapping.cpp
	#FreeImage/WuQuantizer.cpp
	FreeImage/ZLibInterface.cpp
	#FreeImage/tmoColorConvert.cpp
	#FreeImage/tmoDrago03.cpp
	#FreeImage/tmoFattal02.cpp
	#FreeImage/tmoReinhard05.cpp
)

set(fi_CORE_HDRS
	CacheFile.h
	FreeImage.h
	FreeImageIO.h
	Plugin.h
	Quantizers.h
)	

set(fi_OTHER_HDRS	
	ToneMapping.h
	Utilities.h
	DeprecationManager/DeprecationMgr.h
	FreeImage/PSDParser.h
)	

set(fi_OTHER_SRCS
	FreeImage/PSDParser.cpp
	
	FreeImage/PluginBMP.cpp
	FreeImage/PluginCUT.cpp
	FreeImage/PluginDDS.cpp
	FreeImage/PluginEXR.cpp
	FreeImage/PluginG3.cpp
	FreeImage/PluginGIF.cpp
	FreeImage/PluginHDR.cpp
	FreeImage/PluginICO.cpp
	FreeImage/PluginIFF.cpp
	FreeImage/PluginJ2K.cpp
	FreeImage/PluginJP2.cpp
	
	FreeImage/PluginKOALA.cpp
	FreeImage/PluginMNG.cpp
	FreeImage/PluginPCD.cpp
	FreeImage/PluginPCX.cpp
	FreeImage/PluginPFM.cpp
	FreeImage/PluginPICT.cpp
	
	FreeImage/PluginPNM.cpp
	FreeImage/PluginPSD.cpp
	FreeImage/PluginRAS.cpp
	FreeImage/PluginRAW.cpp
	FreeImage/PluginSGI.cpp
	#FreeImage/PluginTARGA.cpp
	FreeImage/PluginTIFF.cpp
	FreeImage/PluginWBMP.cpp
	FreeImage/PluginXBM.cpp
	FreeImage/PluginXPM.cpp
	FreeImage/TIFFLogLuv.cpp
	FreeImage/ToneMapping.cpp
	FreeImage/WuQuantizer.cpp
	
	FreeImage/tmoColorConvert.cpp
	FreeImage/tmoDrago03.cpp
	FreeImage/tmoFattal02.cpp
	FreeImage/tmoReinhard05.cpp
)

if (APPLE)
	source_group(FreeImageCore  FILES ${fi_CORE_SRCS} ${fi_CORE_HDRS})
	source_group(FreeImageOther FILES ${fi_OTHER_SRCS} ${fi_OTHER_HDRS})
else()
	source_group(FreeImage\\Core\\Source FILES ${fi_CORE_SRCS})
	source_group(FreeImage\\Core\\Header FILES ${fi_CORE_HDRS})
	source_group(FreeImage\\Other\\Source FILES ${fi_OTHER_SRCS})
	source_group(FreeImage\\Other\\Header FILES ${fi_OTHER_HDRS})
endif()


# ---------------------------------------------------------
# FreeImageToolKit SubDir
set(fitk_SRCS
	FreeImageToolkit/BSplineRotate.cpp
	FreeImageToolkit/Background.cpp
	FreeImageToolkit/Channels.cpp
	FreeImageToolkit/ClassicRotate.cpp
	FreeImageToolkit/Colors.cpp
	FreeImageToolkit/CopyPaste.cpp
	FreeImageToolkit/Display.cpp
	FreeImageToolkit/Flip.cpp
	FreeImageToolkit/JPEGTransform.cpp
	FreeImageToolkit/MultigridPoissonSolver.cpp
	FreeImageToolkit/Rescale.cpp
	FreeImageToolkit/Resize.cpp
)

set(fitk_HDRS 
	FreeImageToolkit/Filters.h
	FreeImageToolkit/Resize.h
)

if (APPLE)
	source_group(ToolKit FILES ${fitk_SRCS} ${fitk_HDRS})
else()
	source_group(ToolKit\\Source FILES ${fitk_SRCS})
	source_group(ToolKit\\Header FILES ${fitk_HDRS})
endif()	

# ---------------------------------------------------------
# LibJPEG SubDir

set(fi_LibJPEG_HDRS
	LibJPEG/cderror.h
	LibJPEG/cdjpeg.h
	LibJPEG/jchuff.h
	LibJPEG/jconfig.h
	LibJPEG/jdct.h
	LibJPEG/jdhuff.h
	LibJPEG/jerror.h
	LibJPEG/jinclude.h
	LibJPEG/jmemsys.h
	LibJPEG/jmorecfg.h
	LibJPEG/jpegint.h
	LibJPEG/jpeglib.h
	LibJPEG/jversion.h
	LibJPEG/transupp.h
)	

set(fi_LibJPEG_SRCS
	#LibJPEG/cdjpeg.c
	LibJPEG/jaricom.c
	LibJPEG/jcapimin.c
	LibJPEG/jcapistd.c
	LibJPEG/jcarith.c
	LibJPEG/jccoefct.c
	LibJPEG/jccolor.c
	LibJPEG/jcdctmgr.c
	LibJPEG/jchuff.c
	LibJPEG/jcinit.c
	LibJPEG/jcmainct.c
	LibJPEG/jcmarker.c
	LibJPEG/jcmaster.c
	LibJPEG/jcomapi.c
	LibJPEG/jcparam.c
	LibJPEG/jcphuff.c
	LibJPEG/jcprepct.c
	LibJPEG/jcsample.c
	LibJPEG/jctrans.c
	LibJPEG/jdapimin.c
	LibJPEG/jdapistd.c
	LibJPEG/jdarith.c
	LibJPEG/jdatadst.c
	LibJPEG/jdatasrc.c
	LibJPEG/jdcoefct.c
	LibJPEG/jdcolor.c
	LibJPEG/jddctmgr.c
	LibJPEG/jdhuff.c
	LibJPEG/jdinput.c
	LibJPEG/jdmainct.c
	LibJPEG/jdmarker.c
	LibJPEG/jdmaster.c
	LibJPEG/jdmerge.c
	LibJPEG/jdphuff.c
	LibJPEG/jdpostct.c
	LibJPEG/jdsample.c
	LibJPEG/jdtrans.c
	LibJPEG/jerror.c
	LibJPEG/jfdctflt.c
	LibJPEG/jfdctfst.c
	LibJPEG/jfdctint.c
	LibJPEG/jidctflt.c
	LibJPEG/jidctfst.c
	LibJPEG/jidctint.c
	#LibJPEG/jidctred.c
	LibJPEG/jmemansi.c
	#LibJPEG/jmemdos.c
	LibJPEG/jmemmgr.c
	#LibJPEG/jmemname.c
	#LibJPEG/jmemnobs.c
	LibJPEG/jquant1.c
	LibJPEG/jquant2.c
	LibJPEG/jutils.c
	LibJPEG/rdbmp.c
	LibJPEG/rdcolmap.c
	LibJPEG/rdgif.c
	LibJPEG/rdppm.c
	LibJPEG/rdrle.c
	LibJPEG/rdswitch.c
	LibJPEG/rdtarga.c
	LibJPEG/transupp.c
	LibJPEG/wrbmp.c
	LibJPEG/wrgif.c
	LibJPEG/wrppm.c
	LibJPEG/wrrle.c
	LibJPEG/wrtarga.c
)

if (APPLE)
	source_group(LibJPEG FILES ${fi_LibJPEG_SRCS} ${fi_LibJPEG_HDRS})
else()
	source_group(LibJPEG\\Source FILES ${fi_LibJPEG_SRCS})
	source_group(LibJPEG\\Header FILES ${fi_LibJPEG_HDRS})
endif()	


# ---------------------------------------------------------
# LibMNG SubDir
set(fi_LibMNG_SRCS
	LibMNG/libmng_callback_xs.c
	LibMNG/libmng_chunk_descr.c
	LibMNG/libmng_chunk_io.c
	LibMNG/libmng_chunk_prc.c
	LibMNG/libmng_chunk_xs.c
	LibMNG/libmng_cms.c
	LibMNG/libmng_display.c
	LibMNG/libmng_dither.c
	LibMNG/libmng_error.c
	LibMNG/libmng_filter.c
	LibMNG/libmng_hlapi.c
	LibMNG/libmng_jpeg.c
	LibMNG/libmng_object_prc.c
	LibMNG/libmng_pixels.c
	LibMNG/libmng_prop_xs.c
	LibMNG/libmng_read.c
	LibMNG/libmng_trace.c
	LibMNG/libmng_write.c
	LibMNG/libmng_zlib.c
)
set(fi_LibMNG_HDRS
	LibMNG/libmng.h
	LibMNG/libmng_chunk_descr.h
	LibMNG/libmng_chunk_io.h
	LibMNG/libmng_chunk_prc.h
	LibMNG/libmng_chunks.h
	LibMNG/libmng_cms.h
	LibMNG/libmng_conf.h
	LibMNG/libmng_data.h
	LibMNG/libmng_display.h
	LibMNG/libmng_dither.h
	LibMNG/libmng_error.h
	LibMNG/libmng_filter.h
	LibMNG/libmng_jpeg.h
	LibMNG/libmng_memory.h
	LibMNG/libmng_object_prc.h
	LibMNG/libmng_objects.h
	LibMNG/libmng_pixels.h
	LibMNG/libmng_read.h
	LibMNG/libmng_trace.h
	LibMNG/libmng_types.h
	LibMNG/libmng_write.h
	LibMNG/libmng_zlib.h
)

if (APPLE)
	source_group(LibMNG FILES ${fi_LibMNG_SRCS} ${fi_LibMNG_HDRS})
else()
	source_group(LibMNG\\Source FILES ${fi_LibMNG_SRCS})
	source_group(LibMNG\\Header FILES ${fi_LibMNG_HDRS})
endif()	

# ---------------------------------------------------------
# LibOpenJPEG SubDir
set(fi_LibOpenJPEG_SRCS	
	LibOpenJPEG/bio.c
	LibOpenJPEG/cio.c
	LibOpenJPEG/dwt.c
	LibOpenJPEG/event.c
	LibOpenJPEG/image.c
	LibOpenJPEG/j2k.c
	LibOpenJPEG/j2k_lib.c
	LibOpenJPEG/jp2.c
	LibOpenJPEG/jpt.c
	LibOpenJPEG/mct.c
	LibOpenJPEG/mqc.c
	LibOpenJPEG/openjpeg.c
	LibOpenJPEG/pi.c
	LibOpenJPEG/raw.c
	LibOpenJPEG/t1.c
	LibOpenJPEG/t2.c
	LibOpenJPEG/tcd.c
	LibOpenJPEG/tgt.c
)
set(fi_LibOpenJPEG_HDRS	
	LibOpenJPEG/bio.h
	LibOpenJPEG/cio.h
	LibOpenJPEG/dwt.h
	LibOpenJPEG/event.h
	LibOpenJPEG/fix.h
	LibOpenJPEG/image.h
	LibOpenJPEG/int.h
	LibOpenJPEG/j2k.h
	LibOpenJPEG/j2k_lib.h
	LibOpenJPEG/jp2.h
	LibOpenJPEG/jpt.h
	LibOpenJPEG/mct.h
	LibOpenJPEG/mqc.h
	LibOpenJPEG/openjpeg.h
	LibOpenJPEG/opj_includes.h
	LibOpenJPEG/opj_malloc.h
	LibOpenJPEG/pi.h
	LibOpenJPEG/raw.h
	LibOpenJPEG/t1.h
	LibOpenJPEG/t1_luts.h
	LibOpenJPEG/t2.h
	LibOpenJPEG/tcd.h
	LibOpenJPEG/tgt.h
)

if (APPLE)
	source_group(LibOpenJPEG FILES ${fi_LibOpenJPEG_SRCS} ${fi_LibOpenJPEG_HDRS})
else()	
	source_group(LibOpenJPEG\\Source FILES ${fi_LibOpenJPEG_SRCS})
	source_group(LibOpenJPEG\\Header FILES ${fi_LibOpenJPEG_HDRS})
endif()	

# ---------------------------------------------------------
# LibPNG SubDir
set(fi_LibPNG_SRCS
	LibPNG/png.c
	LibPNG/pngerror.c
	LibPNG/pnggccrd.c
	LibPNG/pngget.c
	LibPNG/pngmem.c
	LibPNG/pngpread.c
	LibPNG/pngread.c
	LibPNG/pngrio.c
	LibPNG/pngrtran.c
	LibPNG/pngrutil.c
	LibPNG/pngset.c
	LibPNG/pngtrans.c
	LibPNG/pngvcrd.c
	LibPNG/pngwio.c
	LibPNG/pngwrite.c
	LibPNG/pngwtran.c
	LibPNG/pngwutil.c
)

set(fi_LibPNG_HDRS 
	LibPNG/png.h
	LibPNG/pngconf.h
)

if (APPLE)
	source_group(LibPNG FILES ${fi_LibPNG_SRCS} ${fi_LibPNG_HDRS})
else()
	source_group(LibPNG\\Source FILES ${fi_LibPNG_SRCS})
	source_group(LibPNG\\Header FILES ${fi_LibPNG_HDRS})
endif()	

# ---------------------------------------------------------
# LibTIFF SubDir

set(fi_LibTIFF_SRCS	
	LibTIFF/tif_aux.c
	LibTIFF/tif_close.c
	LibTIFF/tif_codec.c
	LibTIFF/tif_color.c
	LibTIFF/tif_compress.c
	LibTIFF/tif_dir.c
	LibTIFF/tif_dirinfo.c
	LibTIFF/tif_dirread.c
	LibTIFF/tif_dirwrite.c
	LibTIFF/tif_dumpmode.c
	LibTIFF/tif_error.c
	LibTIFF/tif_extension.c
	LibTIFF/tif_fax3.c
	LibTIFF/tif_fax3sm.c
	LibTIFF/tif_flush.c
	LibTIFF/tif_getimage.c
	LibTIFF/tif_jbig.c
	LibTIFF/tif_jpeg.c
	LibTIFF/tif_luv.c
	LibTIFF/tif_lzw.c
	LibTIFF/tif_next.c
	LibTIFF/tif_ojpeg.c
	LibTIFF/tif_open.c
	LibTIFF/tif_packbits.c
	LibTIFF/tif_pixarlog.c
	LibTIFF/tif_predict.c
	LibTIFF/tif_print.c
	LibTIFF/tif_read.c
	LibTIFF/tif_strip.c
	LibTIFF/tif_swab.c
	LibTIFF/tif_thunder.c
	LibTIFF/tif_tile.c
	LibTIFF/tif_version.c
	LibTIFF/tif_warning.c
	LibTIFF/tif_write.c
	LibTIFF/tif_zip.c)

#set(fi_LibTIFF_HDRS	
#	LibTIFF/tiff.h
#	)
	
set(fi_LibTIFF_HDRS
	LibTIFF/t4.h
	LibTIFF/tiff.h
	LibTIFF/tiffconf.vc.h
	LibTIFF/tiffconf.wince.h
	LibTIFF/tiffio.h
	LibTIFF/tiffiop.h
	LibTIFF/tiffvers.h
	LibTIFF/tif_config.h
	LibTIFF/tif_config.vc.h
	LibTIFF/tif_config.wince.h
	LibTIFF/tif_dir.h
	LibTIFF/tif_fax3.h
	LibTIFF/tif_predict.h
	LibTIFF/uvcode.h
)	

if (APPLE)
	source_group(LibTIFF FILES ${fi_LibTIFF_SRCS} ${fi_LibTIFF_HDRS})
else()
	source_group(LibTIFF\\Source FILES ${fi_LibTIFF_SRCS})
	source_group(LibTIFF\\Header FILES ${fi_LibTIFF_HDRS})
endif()	

# ---------------------------------------------------------
# Metadata SubDir

set(fi_Metadata_SRCS
	Metadata/Exif.cpp
	Metadata/FIRational.cpp
	Metadata/FreeImageTag.cpp
	Metadata/IPTC.cpp
	Metadata/TagConversion.cpp
	Metadata/TagLib.cpp
	Metadata/XTIFF.cpp
)

set(fi_Metadata_HDRS 
	Metadata/FIRational.h
	Metadata/FreeImageTag.h
)

if (APPLE)
	source_group(FreeImage/Metadata FILES ${fi_Metadata_SRCS} ${fi_Metadata_HDRS})
else()
	source_group(FreeImage\\Metadata\\Source FILES ${fi_Metadata_SRCS})
	source_group(FreeImage\\Metadata\\Header FILES ${fi_Metadata_HDRS})
endif()	

# ---------------------------------------------------------
# LibRawLite SubDir

set(fi_LibRawLite_SRCS 
	LibRawLite/src/libraw_cxx.cpp
	LibRawLite/src/libraw_c_api.cpp
	LibRawLite/internal/dcraw_common.cpp
	LibRawLite/internal/dcraw_fileio.cpp
	)


set(fi_LibRawLite_HDRS
	LibRawLite/internal/defines.h
	LibRawLite/internal/libraw_internal_funcs.h
	LibRawLite/internal/var_defines.h
	LibRawLite/libraw/libraw.h
	LibRawLite/libraw/libraw_alloc.h
	LibRawLite/libraw/libraw_const.h
	LibRawLite/libraw/libraw_datastream.h
	LibRawLite/libraw/libraw_internal.h
	LibRawLite/libraw/libraw_types.h
	LibRawLite/libraw/libraw_version.h
)	

if (APPLE)
	source_group(LibRawLite FILES ${fi_LibRawLite_SRCS} ${fi_LibRawLite_HDRS})
else()	
	source_group(LibRawLite\\Source FILES ${fi_LibRawLite_SRCS})
	source_group(LibRawLite\\Header FILES ${fi_LibRawLite_HDRS})
endif()	
	

# ---------------------------------------------------------
# OpenEXR SubDir

set(fi_half_SRCS OpenEXR/Half/half.cpp)
set(fi_half_HDRS 
	OpenEXR/Half/half.h
	OpenEXR/Half/halfFunction.h
	OpenEXR/Half/halfLimits.h
)

if (APPLE)
	source_group(OpenEXR/Half FILES ${fi_half_SRCS} ${fi_half_HDRS})
else()	
	source_group(OpenEXR\\Half\\Source FILES ${fi_half_SRCS})
	source_group(OpenEXR\\Half\\Header FILES ${fi_half_HDRS})
endif()	

set(fi_Iex_SRCS
	OpenEXR/Iex/IexBaseExc.cpp
	OpenEXR/Iex/IexThrowErrnoExc.cpp
)

set(fi_Iex_HDRS
	OpenEXR/Iex/Iex.h
	OpenEXR/Iex/IexBaseExc.h
	OpenEXR/Iex/IexErrnoExc.h
	OpenEXR/Iex/IexMacros.h
	OpenEXR/Iex/IexMathExc.h
	OpenEXR/Iex/IexThrowErrnoExc.h
)

if (APPLE)
	source_group(OpenEXR/Iex FILES ${fi_Iex_SRCS} ${fi_Iex_HDRS})
else()	
	source_group(OpenEXR\\Iex\\Source FILES ${fi_Iex_SRCS})
	source_group(OpenEXR\\Iex\\Header FILES ${fi_Iex_HDRS})
endif()	

set(fi_IlmImf_SRCS
	OpenEXR/IlmImf/ImfAttribute.cpp
	OpenEXR/IlmImf/ImfB44Compressor.cpp
	OpenEXR/IlmImf/ImfBoxAttribute.cpp
	OpenEXR/IlmImf/ImfCRgbaFile.cpp
	OpenEXR/IlmImf/ImfChannelList.cpp
	OpenEXR/IlmImf/ImfChannelListAttribute.cpp
	OpenEXR/IlmImf/ImfChromaticities.cpp
	OpenEXR/IlmImf/ImfChromaticitiesAttribute.cpp
	OpenEXR/IlmImf/ImfCompressionAttribute.cpp
	OpenEXR/IlmImf/ImfCompressor.cpp
	OpenEXR/IlmImf/ImfConvert.cpp
	OpenEXR/IlmImf/ImfDoubleAttribute.cpp
	OpenEXR/IlmImf/ImfEnvmap.cpp
	OpenEXR/IlmImf/ImfEnvmapAttribute.cpp
	OpenEXR/IlmImf/ImfFloatAttribute.cpp
	OpenEXR/IlmImf/ImfFrameBuffer.cpp
	OpenEXR/IlmImf/ImfFramesPerSecond.cpp
	OpenEXR/IlmImf/ImfHeader.cpp
	OpenEXR/IlmImf/ImfHuf.cpp
	OpenEXR/IlmImf/ImfIO.cpp
	OpenEXR/IlmImf/ImfInputFile.cpp
	OpenEXR/IlmImf/ImfIntAttribute.cpp
	OpenEXR/IlmImf/ImfKeyCode.cpp
	OpenEXR/IlmImf/ImfKeyCodeAttribute.cpp
	OpenEXR/IlmImf/ImfLineOrderAttribute.cpp
	OpenEXR/IlmImf/ImfLut.cpp
	OpenEXR/IlmImf/ImfMatrixAttribute.cpp
	OpenEXR/IlmImf/ImfMisc.cpp
	OpenEXR/IlmImf/ImfOpaqueAttribute.cpp
	OpenEXR/IlmImf/ImfOutputFile.cpp
	OpenEXR/IlmImf/ImfPizCompressor.cpp
	OpenEXR/IlmImf/ImfPreviewImage.cpp
	OpenEXR/IlmImf/ImfPreviewImageAttribute.cpp
	OpenEXR/IlmImf/ImfPxr24Compressor.cpp
	OpenEXR/IlmImf/ImfRational.cpp
	OpenEXR/IlmImf/ImfRationalAttribute.cpp
	OpenEXR/IlmImf/ImfRgbaFile.cpp
	OpenEXR/IlmImf/ImfRgbaYca.cpp
	OpenEXR/IlmImf/ImfRleCompressor.cpp
	OpenEXR/IlmImf/ImfScanLineInputFile.cpp
	OpenEXR/IlmImf/ImfStandardAttributes.cpp
	OpenEXR/IlmImf/ImfStdIO.cpp
	OpenEXR/IlmImf/ImfStringAttribute.cpp
	OpenEXR/IlmImf/ImfTestFile.cpp
	OpenEXR/IlmImf/ImfThreading.cpp
	OpenEXR/IlmImf/ImfTileDescriptionAttribute.cpp
	OpenEXR/IlmImf/ImfTileOffsets.cpp
	OpenEXR/IlmImf/ImfTiledInputFile.cpp
	OpenEXR/IlmImf/ImfTiledMisc.cpp
	OpenEXR/IlmImf/ImfTiledOutputFile.cpp
	OpenEXR/IlmImf/ImfTiledRgbaFile.cpp
	OpenEXR/IlmImf/ImfTimeCode.cpp
	OpenEXR/IlmImf/ImfTimeCodeAttribute.cpp
	OpenEXR/IlmImf/ImfVecAttribute.cpp
	OpenEXR/IlmImf/ImfVersion.cpp
	OpenEXR/IlmImf/ImfWav.cpp
	OpenEXR/IlmImf/ImfZipCompressor.cpp
	#OpenEXR/IlmImf/b44ExpLogTable.cpp
)
set(fi_IlmImf_HDRS
	OpenEXR/IlmImf/ImfArray.h
	OpenEXR/IlmImf/ImfAttribute.h
	OpenEXR/IlmImf/ImfAutoArray.h
	OpenEXR/IlmImf/ImfB44Compressor.h
	OpenEXR/IlmImf/ImfBoxAttribute.h
	OpenEXR/IlmImf/ImfCRgbaFile.h
	OpenEXR/IlmImf/ImfChannelList.h
	OpenEXR/IlmImf/ImfChannelListAttribute.h
	OpenEXR/IlmImf/ImfChromaticities.h
	OpenEXR/IlmImf/ImfChromaticitiesAttribute.h
	OpenEXR/IlmImf/ImfCompression.h
	OpenEXR/IlmImf/ImfCompressionAttribute.h
	OpenEXR/IlmImf/ImfCompressor.h
	OpenEXR/IlmImf/ImfConvert.h
	OpenEXR/IlmImf/ImfDoubleAttribute.h
	OpenEXR/IlmImf/ImfEnvmap.h
	OpenEXR/IlmImf/ImfEnvmapAttribute.h
	OpenEXR/IlmImf/ImfFloatAttribute.h
	OpenEXR/IlmImf/ImfFrameBuffer.h
	OpenEXR/IlmImf/ImfFramesPerSecond.h
	OpenEXR/IlmImf/ImfHeader.h
	OpenEXR/IlmImf/ImfHuf.h
	OpenEXR/IlmImf/ImfIO.h
	OpenEXR/IlmImf/ImfInputFile.h
	OpenEXR/IlmImf/ImfInt64.h
	OpenEXR/IlmImf/ImfIntAttribute.h
	OpenEXR/IlmImf/ImfKeyCode.h
	OpenEXR/IlmImf/ImfKeyCodeAttribute.h
	OpenEXR/IlmImf/ImfLineOrder.h
	OpenEXR/IlmImf/ImfLineOrderAttribute.h
	OpenEXR/IlmImf/ImfLut.h
	OpenEXR/IlmImf/ImfMatrixAttribute.h
	OpenEXR/IlmImf/ImfMisc.h
	OpenEXR/IlmImf/ImfName.h
	OpenEXR/IlmImf/ImfOpaqueAttribute.h
	OpenEXR/IlmImf/ImfOutputFile.h
	OpenEXR/IlmImf/ImfPixelType.h
	OpenEXR/IlmImf/ImfPizCompressor.h
	OpenEXR/IlmImf/ImfPreviewImage.h
	OpenEXR/IlmImf/ImfPreviewImageAttribute.h
	OpenEXR/IlmImf/ImfPxr24Compressor.h
	OpenEXR/IlmImf/ImfRational.h
	OpenEXR/IlmImf/ImfRationalAttribute.h
	OpenEXR/IlmImf/ImfRgba.h
	OpenEXR/IlmImf/ImfRgbaFile.h
	OpenEXR/IlmImf/ImfRgbaYca.h
	OpenEXR/IlmImf/ImfRleCompressor.h
	OpenEXR/IlmImf/ImfScanLineInputFile.h
	OpenEXR/IlmImf/ImfStandardAttributes.h
	OpenEXR/IlmImf/ImfStdIO.h
	OpenEXR/IlmImf/ImfStringAttribute.h
	OpenEXR/IlmImf/ImfTestFile.h
	OpenEXR/IlmImf/ImfThreading.h
	OpenEXR/IlmImf/ImfTileDescription.h
	OpenEXR/IlmImf/ImfTileDescriptionAttribute.h
	OpenEXR/IlmImf/ImfTileOffsets.h
	OpenEXR/IlmImf/ImfTiledInputFile.h
	OpenEXR/IlmImf/ImfTiledMisc.h
	OpenEXR/IlmImf/ImfTiledOutputFile.h
	OpenEXR/IlmImf/ImfTiledRgbaFile.h
	OpenEXR/IlmImf/ImfTimeCode.h
	OpenEXR/IlmImf/ImfTimeCodeAttribute.h
	OpenEXR/IlmImf/ImfVecAttribute.h
	OpenEXR/IlmImf/ImfVersion.h
	OpenEXR/IlmImf/ImfWav.h
	OpenEXR/IlmImf/ImfXdr.h
	OpenEXR/IlmImf/ImfZipCompressor.h
	#OpenEXR/IlmImf/b44ExpLogTable.h
)

if (APPLE)
	source_group(OpenEXR/IlmImf FILES ${fi_IlmImf_SRCS} ${fi_IlmImf_HDRS})
else()	
	source_group(OpenEXR\\IlmImf\\Source FILES ${fi_IlmImf_SRCS})
	source_group(OpenEXR\\IlmImf\\Header FILES ${fi_IlmImf_HDRS})
endif()	

set(fi_IlmThread_SRCS
	OpenEXR/IlmThread/IlmThreadPool.cpp
)

if (WIN32)
	set(fi_IlmThread_SRCS_PLAT
			OpenEXR/IlmThread/IlmThreadMutexWin32.cpp
			OpenEXR/IlmThread/IlmThreadSemaphoreWin32.cpp
			OpenEXR/IlmThread/IlmThreadWin32.cpp
			)
else (WIN32)			   
	set(fi_IlmThread_SRCS_PLAT
            OpenEXR/IlmThread/IlmThreadSemaphore.cpp
		    OpenEXR/IlmThread/IlmThread.cpp
            OpenEXR/IlmThread/IlmThreadMutex.cpp
			OpenEXR/IlmThread/IlmThreadPosix.cpp
			OpenEXR/IlmThread/IlmThreadMutexPosix.cpp
			OpenEXR/IlmThread/IlmThreadSemaphorePosix.cpp
			OpenEXR/IlmThread/IlmThreadSemaphorePosixCompat.cpp
			)
endif (WIN32)

set(fi_IlmThread_HDRS
	OpenEXR/IlmThread/IlmThread.h
	OpenEXR/IlmThread/IlmThreadMutex.h
	OpenEXR/IlmThread/IlmThreadPool.h
	OpenEXR/IlmThread/IlmThreadSemaphore.h
)

if (APPLE)
	source_group(OpenEXR/IlmThread FILES ${fi_IlmThread_SRCS} ${fi_IlmThread_HDRS} ${fi_IlmThread_SRCS_PLAT})
else()	
	source_group(OpenEXR\\IlmThread\\Source FILES ${fi_IlmThread_SRCS})
	source_group(OpenEXR\\IlmThread\\Source\\Win32 FILES ${fi_IlmThread_SRCS_PLAT})
	source_group(OpenEXR\\IlmThread\\Header FILES ${fi_IlmThread_HDRS})
endif()	

set(fi_Imath_SRCS 
	OpenEXR/Imath/ImathBox.cpp
	OpenEXR/Imath/ImathColorAlgo.cpp
	OpenEXR/Imath/ImathFun.cpp
	OpenEXR/Imath/ImathMatrixAlgo.cpp
	OpenEXR/Imath/ImathRandom.cpp
	OpenEXR/Imath/ImathShear.cpp
	OpenEXR/Imath/ImathVec.cpp
)
set(fi_Imath_HDRS
	OpenEXR/Imath/ImathBox.h
	OpenEXR/Imath/ImathBoxAlgo.h
	OpenEXR/Imath/ImathColor.h
	OpenEXR/Imath/ImathColorAlgo.h
	OpenEXR/Imath/ImathEuler.h
	OpenEXR/Imath/ImathExc.h
	OpenEXR/Imath/ImathFrame.h
	OpenEXR/Imath/ImathFrustum.h
	OpenEXR/Imath/ImathFun.h
	OpenEXR/Imath/ImathGL.h
	OpenEXR/Imath/ImathGLU.h
	OpenEXR/Imath/ImathHalfLimits.h
	OpenEXR/Imath/ImathInt64.h
	OpenEXR/Imath/ImathInterval.h
	OpenEXR/Imath/ImathLimits.h
	OpenEXR/Imath/ImathLine.h
	OpenEXR/Imath/ImathLineAlgo.h
	OpenEXR/Imath/ImathMath.h
	OpenEXR/Imath/ImathMatrix.h
	OpenEXR/Imath/ImathMatrixAlgo.h
	OpenEXR/Imath/ImathPlane.h
	OpenEXR/Imath/ImathPlatform.h
	OpenEXR/Imath/ImathQuat.h
	OpenEXR/Imath/ImathRandom.h
	OpenEXR/Imath/ImathRoots.h
	OpenEXR/Imath/ImathShear.h
	OpenEXR/Imath/ImathSphere.h
	OpenEXR/Imath/ImathVec.h
	OpenEXR/Imath/ImathVecAlgo.h
)

if (APPLE)
	source_group(OpenEXR/Imath FILES ${fi_Imath_SRCS} ${fi_Imath_HDRS})
else()	
	source_group(OpenEXR\\Imath\\Source FILES ${fi_Imath_SRCS})
	source_group(OpenEXR\\Imath\\Header FILES ${fi_Imath_HDRS})
endif()	



if (WIN32)
	if (CMAKE_COMPILER_IS_GNUCXX)
		# FreeImage LibRawLite dependency 
		link_libraries(ws2_32)
	endif (CMAKE_COMPILER_IS_GNUCXX)
endif(WIN32)

# ---------------------------------------------------------
set(CORE_SOURCE
	${fi_CORE_SRCS}			${fi_CORE_HDRS}
	
	${fi_LibJPEG_SRCS}		${fi_LibJPEG_HDRS}
	${fi_LibOpenJPEG_SRCS}	${fi_LibOpenJPEG_HDRS}
	${fi_LibPNG_SRCS}		${fi_LibPNG_HDRS}
	
	${fi_Metadata_SRCS}		${fi_Metadata_HDRS}
	${fitk_SRCS}			${fitk_HDRS}
)

set(OTHER_SOURCE
	${fi_OTHER_SRCS}		${fi_OTHER_HDRS}
	${fi_LibTIFF_SRCS}		${fi_LibTIFF_HDRS}
	
	#${fi_LibJPEG_SRCS}
	#${fi_LibJPEG_PLAT}
	${fi_LibMNG_SRCS}		${fi_LibMNG_HDRS}
	#${fi_LibOpenJPEG_SRCS}
	#${fi_LibPNG_SRCS}
	
	
	
	
	
	# ${fi_rLibRawLite_SRCS} ${fi_LibRawLite_HDRS}
)
	
set(EXR_SOURCE
	${fi_half_SRCS}			${fi_half_HDRS}	
	${fi_Iex_SRCS}			${fi_Iex_HDRS}	
	${fi_IlmImf_SRCS}		${fi_IlmImf_HDRS}
	${fi_Imath_SRCS} 		${fi_Imath_HDRS}
	
	${fi_IlmThread_SRCS}	${fi_IlmThread_HDRS}	${fi_IlmThread_SRCS_PLAT}
)	
	
set(ALL_SOURCE	${CORE_SOURCE})

	
if (OGREKIT_MINIMAL_FREEIMAGE_CODEC)
	add_definitions(-DOGREKIT_MINIMAL_FREEIMAGE_CODEC=1)
else()
	list(APPEND ALL_SOURCE ${OTHER_SOURCE})
	list(APPEND ALL_SOURCE ${EXR_SOURCE})
endif()	

add_library(${OGREKIT_FREEIMAGE_TARGET}	${ALL_SOURCE})

config_ogrekit_target(${OGREKIT_FREEIMAGE_TARGET} TRUE)

if (OGREKIT_CREATE_OGRE_DEPENDENCY_DIR)
	file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/FreeImage.h DESTINATION ${OGRE_PREBUILT_DEPENDENCY_INCLUDE_DIR})
	GET_TARGET_PROPERTY(FILEPATH ${OGREKIT_FREEIMAGE_TARGET} LOCATION)
	ADD_CUSTOM_COMMAND(TARGET ${OGREKIT_FREEIMAGE_TARGET} POST_BUILD
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${FILEPATH} ${OGRE_PREBUILT_DEPENDENCY_LIB_DIR}/libfreeimage.a
		COMMAND ${CMAKE_COMMAND} ARGS -E copy_if_different ${FILEPATH} ${OGRE_PREBUILT_DEPENDENCY_LIB_DIR}/libFreeImage.a
	)
endif()
