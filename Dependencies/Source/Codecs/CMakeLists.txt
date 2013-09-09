include_directories(include Vorbis)

set(OGG
	include/ogg/ogg.h
	include/ogg/os_types.h
	Ogg/bitwise.c
	Ogg/framing.c
)

set(VORBIS
	include/vorbis/codec.h
	include/vorbis/vorbisenc.h
	include/vorbis/vorbisfile.h
	Vorbis/backends.h
	Vorbis/bitrate.h
	Vorbis/codebook.h
	Vorbis/codec_internal.h
	Vorbis/envelope.h
	Vorbis/highlevel.h
	Vorbis/lookup.h
	Vorbis/lookup_data.h
	Vorbis/lpc.h
	Vorbis/lsp.h
	Vorbis/masking.h
	Vorbis/mdct.h
	Vorbis/misc.h
	Vorbis/os.h
	Vorbis/psy.h
	Vorbis/registry.h
	Vorbis/scales.h
	Vorbis/smallft.h
	Vorbis/window.h

	Vorbis/analysis.c
	#Vorbis/barkmel.c
	Vorbis/bitrate.c
	Vorbis/block.c
	Vorbis/codebook.c
	Vorbis/envelope.c
	Vorbis/floor0.c
	Vorbis/floor1.c
	Vorbis/info.c
	Vorbis/lookup.c
	Vorbis/lpc.c
	Vorbis/lsp.c
	Vorbis/mapping0.c
	Vorbis/mdct.c
	Vorbis/psy.c
	Vorbis/registry.c
	Vorbis/res0.c
	Vorbis/sharedbook.c
	Vorbis/smallft.c
	Vorbis/synthesis.c
	Vorbis/vorbisenc.c
	Vorbis/vorbisfile.c
	Vorbis/window.c
)

add_library(${OGREKIT_OGGVORBIS_TARGET} ${OGG} ${VORBIS})

config_ogrekit_target(${OGREKIT_OGGVORBIS_TARGET} TRUE)
