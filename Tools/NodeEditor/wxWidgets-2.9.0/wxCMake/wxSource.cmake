## ======================================================================
##                                     FILES
##
##   Files are listed in variables according to this scheme:
##   * each group has _SRC and _HDR variable, for sources and headers
##     respectively
##   * file groups are:
##
##     BASE_CMN                      common base files (all platforms)
##     GUI_CMN                       common GUI files (all platforms)
##     BASE_AND_GUI_CMN_SRC          files that must be compiled both into GUI
##                                   and base library (all platforms)
##
##     BASE_${platform}             base files used on platform
##     BASE_AND_GUI_${platform}_SRC  base & gui files used on platform
##
##     $(port}LOWLEVEL               low-level GUI classes
##     $(port}                       high-level files for a port
##     XWIN_LOWLEVEL                 low-level classes for X Window System
##                                   ports (wxGTK, wxMotif, wxX11)
##
##     MSW_DESKTOP                   files used only on desktop Windows
##                                   versions and not under Windows CE
##
##     xxx_NATIVE                    native files not used by wxUniv (used as
##                                   complement of LOWLEVEL when most of the
##                                   files are lowlevel and only few are native)
##
##     HTML                          wxHTML files
##     XRC                           wxXRC files
##
##   * one file may be listed in several groups
##
##   ======================================================================



## ======================================================================
##                               wxBase
## ======================================================================

## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
##                               UNIX
## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

## Files used on all Unix systems, including Darwin
set(BASE_UNIX_AND_DARWIN_SRC
    src/common/fdiodispatcher.cpp
    src/common/selectdispatcher.cpp
    src/unix/epolldispatcher.cpp
    src/unix/appunix.cpp
    src/unix/evtloopunix.cpp
    src/unix/dir.cpp
    src/unix/dlunix.cpp
    src/unix/fdiounix.cpp
    src/unix/snglinst.cpp
    src/unix/stdpaths.cpp
    src/unix/timerunx.cpp
    src/unix/threadpsx.cpp
    src/unix/stackwalk.cpp
    src/unix/utilsunx.cpp
)

set(BASE_UNIX_AND_DARWIN_HDR
    wx/unix/app.h
    wx/unix/apptbase.h
    wx/unix/apptrait.h
    wx/unix/chkconf.h
    wx/unix/evtloop.h
    wx/unix/pipe.h
    wx/unix/stdpaths.h
    wx/unix/stackwalk.h
    wx/unix/tls.h
)

##
##  Files used on all Unix systems, including Darwin with any port but wxMac
##  (this is really an aberration, we should also use these files for wxMac,
##   it's not done because of some OS 9 support leftovers only now)

set(BASE_UNIX_AND_DARWIN_NOTWXMAC_SRC
    ${BASE_UNIX_AND_DARWIN_SRC}
    src/unix/mimetype.cpp
)

set(BASE_UNIX_AND_DARWIN_NOTWXMAC_HDR
    ${BASE_UNIX_AND_DARWIN_HDR}
    wx/unix/execute.h
    wx/unix/mimetype.h
)

##
##  Files used on all non-Darwin Unix systems (because Darwin provides other,
##  better, native equivalents of the functionality they implement (e.g.
##  CFSocket instead of BSD sockets) or doesn't provide it at all (epoll,
##  backtrace())

set(BASE_UNIX_SRC
    ${BASE_UNIX_AND_DARWIN_NOTWXMAC_SRC}
)
set(BASE_UNIX_HDR
    ${BASE_UNIX_AND_DARWIN_NOTWXMAC_HDR}
)

## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
##                             Windows
## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
set(BASE_WIN32_SRC
    src/msw/basemsw.cpp
    src/msw/crashrpt.cpp
    src/msw/debughlp.cpp
    src/msw/dde.cpp
    src/msw/dir.cpp
    src/msw/dlmsw.cpp
    src/msw/mimetype.cpp
    src/msw/power.cpp
    src/msw/regconf.cpp
    src/msw/registry.cpp
    src/msw/snglinst.cpp
    src/msw/stackwalk.cpp
    src/msw/stdpaths.cpp
    src/msw/thread.cpp
    src/msw/timer.cpp
    src/msw/utils.cpp
    src/msw/utilsexc.cpp
)
set(BASE_AND_GUI_WIN32_SRC
    src/msw/evtloop.cpp
    src/msw/main.cpp
    src/msw/mslu.cpp
    src/msw/volume.cpp
)
##set(BASE_WINCE_SRC
##   <if cond="TOOLKIT=='WINCE'">src/msw/wince/time.cpp</if>
##)
set(BASE_WIN32_HDR
    wx/msw/apptrait.h
    wx/msw/apptbase.h
    wx/msw/chkconf.h
    wx/msw/crashrpt.h
    wx/msw/dde.h
    wx/msw/debughlp.h
    wx/msw/gccpriv.h
    wx/msw/libraries.h
    wx/msw/mimetype.h
    wx/msw/mslu.h
    wx/msw/private.h
    wx/msw/regconf.h
    wx/msw/registry.h
    wx/msw/seh.h
    wx/msw/stackwalk.h
    wx/msw/stdpaths.h
    wx/msw/winundef.h
    wx/msw/wrapcctl.h
    wx/msw/wrapcdlg.h
    wx/msw/wrapwin.h
)
##set(BASE_WINCE_HDR
##    <if cond="TOOLKIT=='WINCE'">
##        wx/msw/wince/time.h
##        wx/msw/wince/chkconf.h
##    </if>
##)

## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
##                                Mac
## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

## Used on Mac OS X wxMac base, and Mac OS X darwin base
set(BASE_COREFOUNDATION_SRC
    src/osx/core/cfstring.cpp
    src/osx/core/stdpaths_cf.cpp
    src/osx/core/strconv_cf.cpp
    src/osx/core/utilsexc_base.cpp
)
set(BASE_COREFOUNDATION_HDR
    wx/osx/core/cfdataref.h
    wx/osx/core/cfref.h
    wx/osx/core/cfstring.h
    wx/osx/core/hid.h
    wx/osx/core/private.h
    wx/osx/core/stdpaths.h
    wx/osx/core/private/strconv_cf.h
)

## Base files used by OS X ports (not Carbon)
set(BASE_OSX_SHARED_SRC
    src/osx/core/mimetype.cpp
    ${BASE_COREFOUNDATION_SRC}
    ${BASE_UNIX_AND_DARWIN_SRC}
)
set(BASE_OSX_SHARED_HDR
    ${BASE_COREFOUNDATION_HDR}
    ${BASE_UNIX_AND_DARWIN_HDR}
)
## Base and GUI files used by OS X
set(BASE_AND_GUI_OSX_CARBON_SRC
    src/osx/carbon/utils.cpp
    src/osx/carbon/uma.cpp
)

set(BASE_AND_GUI_OSX_COCOA_SRC
    src/osx/cocoa/utils.mm
)

## Base files used by non-wxMac OS X builds
set(BASE_OSX_NOTWXMAC_SRC
    ${BASE_UNIX_AND_DARWIN_NOTWXMAC_SRC}
    ${BASE_COREFOUNDATION_SRC}
)
set(BASE_OSX_NOTWXMAC_HDR
    ${BASE_UNIX_AND_DARWIN_NOTWXMAC_HDR}
    ${BASE_COREFOUNDATION_HDR}
)

## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
##                               OS/2
## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

set(BASE_OS2_SRC
    src/common/fdiodispatcher.cpp
    src/common/selectdispatcher.cpp
    src/unix/appunix.cpp
    src/unix/evtloopunix.cpp
    src/unix/timerunx.cpp
    src/os2/dir.cpp
    src/os2/mimetype.cpp
    src/os2/snglinst.cpp
    src/os2/stdpaths.cpp
    src/os2/thread.cpp
    src/os2/utils.cpp
    src/os2/utilsexc.cpp
)
set(BASE_AND_GUI_OS2_SRC
)
set(BASE_OS2_HDR
    wx/unix/app.h
    wx/os2/apptbase.h
    wx/os2/apptrait.h
    wx/unix/evtloop.h
    wx/os2/mimetype.h
    wx/os2/private.h
    wx/os2/stdpaths.h
    wx/os2/chkconf.h
    wx/os2/wxrsc.h
)

## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
##                              PalmOS
## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

set(BASE_PALMOS_SRC
    src/palmos/base.cpp
    src/palmos/dir.cpp
    src/palmos/prefconf.cpp
    src/palmos/snglinst.cpp
    src/palmos/stdall.c
    src/palmos/thread.cpp
    src/palmos/utils.cpp
    src/palmos/utilsexc.cpp
)

set(BASE_AND_GUI_PALMOS_SRC
    src/palmos/main.cpp
    src/palmos/volume.cpp
)

set(BASE_PALMOS_HDR
    wx/palmos/apptbase.h
    wx/palmos/apptrait.h
    wx/palmos/chkconf.h
    wx/palmos/mimetype.h
    wx/palmos/missing.h
    wx/palmos/stdpaths.h
)

## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
##                               MSDOS
## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

set(BASE_MSDOS_SRC
    src/msdos/dir.cpp
    src/msdos/mimetype.cpp
    src/msdos/utilsdos.cpp
)
set(BASE_AND_GUI_MSDOS_SRC
)
set(BASE_MSDOS_HDR
    wx/msdos/mimetype.h
)

## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
##                              Common
## =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
set(BASE_CMN_SRC
    src/common/appbase.cpp
    src/common/arcall.cpp
    src/common/arcfind.cpp
    src/common/archive.cpp
    src/common/arrstr.cpp
    src/common/base64.cpp
    src/common/clntdata.cpp
    src/common/cmdline.cpp
    src/common/config.cpp
    src/common/convauto.cpp
    src/common/datetime.cpp
    src/common/datetimefmt.cpp
    src/common/datstrm.cpp
    src/common/dircmn.cpp
    src/common/dynarray.cpp
    src/common/dynlib.cpp
    src/common/dynload.cpp
    src/common/encconv.cpp
    src/common/evtloopcmn.cpp
    #src/common/extended.c
    src/common/ffile.cpp
    src/common/file.cpp
    src/common/fileback.cpp
    src/common/fileconf.cpp
    src/common/filefn.cpp
    src/common/filename.cpp
    src/common/filesys.cpp
    src/common/filtall.cpp
    src/common/filtfind.cpp
    src/common/fmapbase.cpp
    src/common/fs_arc.cpp
    src/common/fs_filter.cpp
    src/common/hash.cpp
    src/common/hashmap.cpp
    src/common/init.cpp
    src/common/intl.cpp
    src/common/ipcbase.cpp
    src/common/list.cpp
    src/common/log.cpp
    src/common/longlong.cpp
    src/common/memory.cpp
    src/common/mimecmn.cpp
    src/common/module.cpp
    src/common/mstream.cpp
    src/common/object.cpp
    src/common/platinfo.cpp
    src/common/powercmn.cpp
    src/common/process.cpp
    src/common/regex.cpp
    src/common/stdpbase.cpp
    src/common/sstream.cpp
    src/common/stopwatch.cpp
    src/common/strconv.cpp
    src/common/stream.cpp
    src/common/string.cpp
    src/common/stringimpl.cpp
    src/common/stringops.cpp
    src/common/strvararg.cpp
    src/common/sysopt.cpp
    src/common/tarstrm.cpp
    src/common/textbuf.cpp
    src/common/textfile.cpp
    src/common/timercmn.cpp
    src/common/timerimpl.cpp
    src/common/tokenzr.cpp
    src/common/txtstrm.cpp
    src/common/unichar.cpp
    src/common/uri.cpp
    src/common/ustring.cpp
    src/common/variant.cpp
    src/common/wfstream.cpp
    src/common/wxcrt.cpp
    src/common/wxprintf.cpp
    src/common/xlocale.cpp
    src/common/xti.cpp
    src/common/xtistrm.cpp
    src/common/zipstrm.cpp
    src/common/zstream.cpp
)
set(BASE_AND_GUI_CMN_SRC
    src/common/event.cpp
    src/common/fs_mem.cpp
    src/common/msgout.cpp
    src/common/utilscmn.cpp
)
set(BASE_CMN_HDR
    wx/afterstd.h
    wx/anystr.h
    wx/app.h
    wx/apptrait.h
    wx/archive.h
    wx/arrimpl.cpp
    wx/arrstr.h
    wx/atomic.h
    wx/base64.h
    wx/beforestd.h
    wx/buffer.h
    wx/build.h
    wx/chartype.h
    wx/checkeddelete.h
    wx/chkconf.h
    wx/clntdata.h
    wx/cmdargs.h
    wx/cmdline.h
    wx/confbase.h
    wx/config.h
    wx/convauto.h
    wx/containr.h
    wx/cpp.h
    wx/crt.h
    wx/datetime.h
    wx/datstrm.h
    wx/dde.h
    wx/debug.h
    wx/defs.h
    wx/dir.h
    wx/dlimpexp.h
    wx/dlist.h
    wx/dynarray.h
    wx/dynlib.h
    wx/dynload.h
    wx/encconv.h
    wx/event.h
    wx/except.h
    wx/features.h
    wx/flags.h
    wx/ffile.h
    wx/file.h
    wx/fileconf.h
    wx/filefn.h
    wx/filename.h
    wx/filesys.h
    wx/fontenc.h
    wx/fontmap.h
    wx/fs_arc.h
    wx/fs_filter.h
    wx/fs_mem.h
    wx/fs_zip.h
    wx/hash.h
    wx/hashmap.h
    wx/hashset.h
    wx/html/forcelnk.h
    wx/iconloc.h
    wx/init.h
    wx/intl.h
    wx/iosfwrap.h
    wx/ioswrap.h
    wx/ipc.h
    wx/ipcbase.h
    wx/kbdstate.h
    wx/link.h
    wx/list.h
    wx/listimpl.cpp
    wx/log.h
    wx/longlong.h
    wx/math.h
    wx/memconf.h
    wx/memory.h
    wx/memtext.h
    wx/mimetype.h
    wx/module.h
    wx/mousestate.h
    wx/msgout.h
    wx/mstream.h
    wx/object.h
    wx/platform.h
    wx/platinfo.h
    wx/power.h
    wx/process.h
    wx/ptr_scpd.h
    wx/ptr_shrd.h
    wx/recguard.h
    wx/regex.h
    wx/scopedarray.h
    wx/scopedptr.h
    wx/scopeguard.h
    wx/sharedptr.h
    wx/snglinst.h
    wx/sstream.h
    wx/stack.h
    wx/stackwalk.h
    wx/stdpaths.h
    wx/stockitem.h
    wx/stopwatch.h
    wx/strconv.h
    wx/stream.h
    wx/string.h
    wx/stringimpl.h
    wx/stringops.h
    wx/strvararg.h
    wx/sysopt.h
    wx/tarstrm.h
    wx/textbuf.h
    wx/textfile.h
    wx/thread.h
    wx/thrimpl.cpp
    wx/timer.h
    wx/tls.h
    wx/tokenzr.h
    wx/tracker.h
    wx/txtstrm.h
    wx/types.h
    wx/unichar.h
    wx/uri.h
    wx/ustring.h
    wx/utils.h
    wx/variant.h
    wx/vector.h
    wx/version.h
    wx/volume.h
    wx/weakref.h
    wx/wfstream.h
    wx/wx.h
    wx/wxchar.h
    wx/wxcrt.h
    wx/wxcrtbase.h
    wx/wxcrtvararg.h
    wx/wxprec.h
    wx/xlocale.h
    wx/xti.h
    wx/xtistrm.h
    wx/zipstrm.h
    wx/zstream.h
    wx/meta/convertible.h
    wx/meta/if.h
    wx/meta/int2type.h
    wx/meta/movable.h
)


## ======================================================================
##                          wxNet (part of wxBase)
## ======================================================================

set(NET_UNIX_SRC
    src/common/socketiohandler.cpp
    src/unix/sockunix.cpp
)

set(NET_OSX_SRC
    src/osx/core/sockosx.cpp
)

set(NET_WIN32_SRC
    src/msw/sockmsw.cpp
    src/msw/urlmsw.cpp
)
set(NET_WIN32_HDR
    wx/msw/private/sockmsw.h
)

set(NET_WINCE_SRC
    src/msw/wince/net.cpp
)
set(NET_WINCE_HDR
    wx/msw/wince/net.h
)

set(NET_OS2_SRC
    src/unix/sockunix.cpp
)

set(NET_CMN_SRC
    src/common/fs_inet.cpp
    src/common/ftp.cpp
    src/common/http.cpp
    src/common/protocol.cpp
    src/common/sckaddr.cpp
    src/common/sckfile.cpp
    src/common/sckipc.cpp
    src/common/sckstrm.cpp
    src/common/socket.cpp
    src/common/url.cpp
)
set(NET_CMN_HDR
    wx/fs_inet.h
    wx/protocol/file.h
    wx/protocol/ftp.h
    wx/protocol/http.h
    wx/protocol/log.h
    wx/protocol/protocol.h
    wx/sckaddr.h
    wx/sckipc.h
    wx/sckstrm.h
    wx/socket.h
    wx/url.h
)

## ======================================================================
##                          wxQA (non GUI library)
## ======================================================================

set(QA_SRC
    src/common/debugrpt.cpp
    src/generic/dbgrptg.cpp
)

set(QA_HDR
    wx/debugrpt.h
)

## ======================================================================
##                            Common GUI files
## ======================================================================

set(GUI_CMN_SRC
    src/common/accelcmn.cpp
    src/common/accesscmn.cpp
    src/common/anidecod.cpp
    src/common/appcmn.cpp
    src/common/artprov.cpp
    src/common/artstd.cpp
    src/common/bmpbase.cpp
    src/common/bookctrl.cpp
    src/common/btncmn.cpp
    src/common/cairo.cpp
    src/common/choiccmn.cpp
    src/common/clipcmn.cpp
    src/common/clrpickercmn.cpp
    src/common/colourcmn.cpp
    src/common/combocmn.cpp
    src/common/cmdproc.cpp
    src/common/cmndata.cpp
    src/common/containr.cpp
    src/common/cshelp.cpp
    src/common/ctrlcmn.cpp
    src/common/ctrlsub.cpp
    src/common/datacmn.cpp
    src/common/dcbase.cpp
    src/common/dcbufcmn.cpp
    src/common/dcgraph.cpp
    src/common/dcsvg.cpp
    src/common/dlgcmn.cpp
    src/common/dndcmn.cpp
    src/common/dobjcmn.cpp
    src/common/docmdi.cpp
    src/common/docview.cpp
    src/common/dpycmn.cpp
    src/common/dseldlg.cpp
    src/common/effects.cpp
    src/common/fddlgcmn.cpp
    src/common/filectrlcmn.cpp
    src/common/filepickercmn.cpp
    src/common/fontpickercmn.cpp
    src/common/fldlgcmn.cpp
    src/common/fontcmn.cpp
    src/common/fontenumcmn.cpp
    src/common/fontmap.cpp
    src/common/fontutilcmn.cpp
    src/common/framecmn.cpp
    src/common/gaugecmn.cpp
    src/common/gbsizer.cpp
    src/common/gdicmn.cpp
    src/common/geometry.cpp
    src/common/gifdecod.cpp
    src/common/graphcmn.cpp
    src/common/headercolcmn.cpp
    src/common/headerctrlcmn.cpp
    src/common/helpbase.cpp
    src/common/iconbndl.cpp
    src/common/imagall.cpp
    src/common/imagbmp.cpp
    src/common/image.cpp
    src/common/imagfill.cpp
    src/common/imaggif.cpp
    src/common/imagiff.cpp
    src/common/imagjpeg.cpp
    src/common/imagpcx.cpp
    src/common/imagpng.cpp
    src/common/imagpnm.cpp
    src/common/imagtga.cpp
    src/common/imagtiff.cpp
    src/common/imagxpm.cpp
    src/common/layout.cpp
    src/common/lboxcmn.cpp
    src/common/listctrlcmn.cpp
    src/common/matrix.cpp
    src/common/menucmn.cpp
    src/common/nbkbase.cpp
    src/common/overlaycmn.cpp
    src/common/paper.cpp
    src/common/persist.cpp
    src/common/pickerbase.cpp
    src/common/popupcmn.cpp
    src/common/prntbase.cpp
    src/common/quantize.cpp
    src/common/radiocmn.cpp
    src/common/rearrangectrl.cpp
    src/common/rendcmn.cpp
    src/common/rgncmn.cpp
    src/common/settcmn.cpp
    src/common/sizer.cpp
    src/common/spinctrlcmn.cpp
    src/common/srchcmn.cpp
    src/common/statbar.cpp
    src/common/stattextcmn.cpp
    src/common/stockitem.cpp
    src/common/tbarbase.cpp
    src/common/textcmn.cpp
    src/common/textentrycmn.cpp
    src/common/toplvcmn.cpp
    src/common/treebase.cpp
    src/common/valgen.cpp
    src/common/validate.cpp
    src/common/valtext.cpp
    src/common/wincmn.cpp
    src/common/windowid.cpp
    src/common/wrapsizer.cpp
    src/common/xpmdecod.cpp
    src/generic/busyinfo.cpp
    src/generic/buttonbar.cpp
    src/generic/choicdgg.cpp
    src/generic/choicbkg.cpp
    src/generic/combog.cpp
    src/generic/dcpsg.cpp
    src/generic/dirctrlg.cpp
    src/generic/dragimgg.cpp
    src/generic/filectrlg.cpp
    src/generic/headerctrlg.cpp
    src/generic/listbkg.cpp
    src/generic/logg.cpp
    src/generic/numdlgg.cpp
    src/generic/panelg.cpp
    src/generic/progdlgg.cpp
    src/generic/printps.cpp
    src/generic/renderg.cpp
    src/generic/scrlwing.cpp
    src/generic/selstore.cpp
    src/generic/spinctlg.cpp
    src/generic/splitter.cpp
    src/generic/srchctlg.cpp
    src/generic/statbmpg.cpp
    src/generic/stattextg.cpp
    src/generic/textdlgg.cpp
    src/generic/tipwin.cpp
    src/generic/toolbkg.cpp
    src/generic/treectlg.cpp
    src/generic/treebkg.cpp
    src/generic/vlbox.cpp
    src/generic/vscroll.cpp
)
set(GUI_CMN_HDR
    wx/bmpbuttn.h
    wx/brush.h
    wx/button.h
    wx/cairo.h
    wx/checkbox.h
    wx/checklst.h
    wx/choicdlg.h
    wx/choice.h
    wx/cmndata.h
    wx/collpane.h
    wx/combo.h
    wx/combobox.h
    wx/control.h
    wx/ctrlsub.h
    wx/cursor.h
    wx/dc.h
    wx/dcclient.h
    wx/dcgraph.h
    wx/dcmemory.h
    wx/dcprint.h
    wx/dcscreen.h
    wx/dcsvg.h
    wx/dialog.h
    wx/dirdlg.h
    wx/dragimag.h
    wx/encinfo.h
    wx/filedlg.h
    wx/frame.h
    wx/gauge.h
    wx/gbsizer.h
    wx/gdicmn.h
    wx/generic/accel.h
    wx/generic/buttonbar.h
    wx/generic/choicdgg.h
    wx/generic/colrdlgg.h
    wx/generic/combo.h
    wx/generic/dcpsg.h
    wx/generic/dirctrlg.h
    wx/generic/dragimgg.h
    wx/generic/filectrlg.h
    wx/generic/headerctrlg.h
    wx/generic/logg.h
    wx/generic/msgdlgg.h
    wx/generic/numdlgg.h
    wx/generic/notebook.h
    wx/generic/panelg.h
    wx/generic/prntdlgg.h
    wx/generic/printps.h
    wx/generic/progdlgg.h
    wx/generic/scrolwin.h
    wx/generic/spinctlg.h
    wx/generic/splitter.h
    wx/generic/srchctlg.h
    wx/generic/statbmpg.h
    wx/generic/stattextg.h
    wx/generic/textdlgg.h
    wx/generic/treectlg.h
    wx/graphics.h
    wx/headercol.h
    wx/headerctrl.h
    wx/helphtml.h
    wx/icon.h
    wx/layout.h
    wx/listbox.h
    wx/mdi.h
    wx/menu.h
    wx/msgdlg.h
    wx/nativewin.h
    wx/numdlg.h
    wx/overlay.h
    wx/palette.h
    wx/panel.h
    wx/pen.h
    wx/position.h
    wx/radiobox.h
    wx/radiobut.h
    wx/rearrangectrl.h
    wx/renderer.h
    wx/scrolbar.h
    wx/scrolbar.h
    wx/scrolwin.h
    wx/selstore.h
    wx/settings.h
    wx/sizer.h
    wx/slider.h
    wx/statbmp.h
    wx/statbox.h
    wx/stattext.h
    wx/statusbr.h
    wx/textctrl.h
    wx/textdlg.h
    wx/textentry.h
    wx/toolbar.h
    wx/validate.h
    wx/valtext.h
    wx/window.h
    wx/windowid.h
    wx/wrapsizer.h
    wx/wupdlock.h

    wx/accel.h
    wx/access.h
    wx/anidecod.h
    wx/animdecod.h
    wx/artprov.h
    wx/bitmap.h
    wx/bookctrl.h
    wx/busyinfo.h
    wx/generic/busyinfo.h
    wx/caret.h
    wx/choicebk.h
    wx/clipbrd.h
    wx/clrpicker.h
    wx/cmdproc.h
    wx/colordlg.h
    wx/colour.h
    wx/cshelp.h
    wx/dataobj.h
    wx/dcmirror.h
    wx/dcps.h
    wx/dialup.h
    wx/dirctrl.h
    wx/display.h
    wx/display_impl.h
    wx/dnd.h
    wx/docmdi.h
    wx/docview.h
    wx/effects.h
    wx/evtloop.h
    wx/fdrepdlg.h
    wx/filectrl.h
    wx/filepicker.h
    wx/fontpicker.h
    wx/fmappriv.h
    wx/font.h
    wx/fontdlg.h
    wx/fontenum.h
    wx/fontutil.h
    wx/gdiobj.h
    wx/geometry.h
    wx/gifdecod.h
    wx/help.h
    wx/helpbase.h
    wx/helpwin.h
    wx/iconbndl.h
    wx/imagbmp.h
    wx/image.h
    wx/imaggif.h
    wx/imagiff.h
    wx/imagjpeg.h
    wx/imaglist.h
    wx/imagpcx.h
    wx/imagpng.h
    wx/imagpnm.h
    wx/imagtga.h
    wx/imagtiff.h
    wx/imagxpm.h
    wx/listbase.h
    wx/listbook.h
    wx/listctrl.h
    wx/matrix.h
    wx/menuitem.h
    wx/metafile.h
    wx/minifram.h
    wx/nonownedwnd.h
    wx/notebook.h
    wx/ownerdrw.h
    wx/paper.h
    wx/persist.h
    wx/persist/bookctrl.h
    wx/persist/toplevel.h
    wx/persist/treebook.h
    wx/persist/window.h
    wx/pickerbase.h
    wx/popupwin.h
    wx/print.h
    wx/printdlg.h
    wx/prntbase.h
    wx/progdlg.h
    wx/quantize.h
    wx/rawbmp.h
    wx/region.h
    wx/scopeguard.h
    wx/spinbutt.h
    wx/spinctrl.h
    wx/splitter.h
    wx/srchctrl.h
    wx/statline.h
    wx/tbarbase.h
    wx/tglbtn.h
    wx/tipwin.h
    wx/toolbook.h
    wx/tooltip.h
    wx/toplevel.h
    wx/treebase.h
    wx/treebook.h
    wx/treectrl.h
    wx/valgen.h
    wx/vidmode.h
    wx/vlbox.h
    wx/vms_x_fix.h
    wx/vscroll.h
    wx/xpmdecod.h
    wx/xpmhand.h
)

## ======================================================================
##                            Common Unix files:
## ======================================================================

set(UNIX_SRC
    src/unix/apptraits.cpp
)

## ======================================================================
##                            Common X11 code:
## ======================================================================

set(XWIN_LOWLEVEL_SRC
    src/generic/caret.cpp
    src/generic/imaglist.cpp
    src/unix/dialup.cpp
    src/unix/fontenum.cpp
    src/unix/fontutil.cpp
    src/unix/utilsx11.cpp
    src/unix/displayx11.cpp
)
set(XWIN_LOWLEVEL_HDR
    wx/generic/caret.h
    wx/generic/imaglist.h
    wx/unix/fontutil.h
    wx/unix/utilsx11.h
)


## ======================================================================
##                                wxGTK
## ======================================================================

set(GTK_LOWLEVEL_SRC
    ${XWIN_LOWLEVEL_SRC}
    src/generic/icon.cpp
    src/generic/paletteg.cpp
    src/gtk/app.cpp
    src/gtk/assertdlg_gtk.c
    src/gtk/bitmap.cpp
    src/gtk/brush.cpp
    src/gtk/clipbrd.cpp
    src/gtk/colour.cpp
    src/gtk/cursor.cpp
    src/gtk/dataobj.cpp
    src/gtk/dc.cpp
    src/gtk/dcclient.cpp
    src/gtk/dcmemory.cpp
    src/gtk/dcscreen.cpp
    src/gtk/dnd.cpp
    src/gtk/evtloop.cpp
    src/gtk/filectrl.cpp
    src/gtk/font.cpp
    src/gtk/sockgtk.cpp
    src/gtk/minifram.cpp
    src/gtk/pen.cpp
    src/gtk/popupwin.cpp
    src/gtk/private.cpp
    src/gtk/region.cpp
    src/gtk/renderer.cpp
    src/gtk/settings.cpp
    src/gtk/timer.cpp
    src/gtk/tooltip.cpp
    src/gtk/toplevel.cpp
    src/gtk/utilsgtk.cpp
    src/gtk/win_gtk.cpp
    src/gtk/window.cpp
)
set(GTK_LOWLEVEL_HDR
    ${XWIN_LOWLEVEL_HDR}
    wx/generic/icon.h
    wx/generic/paletteg.h
    wx/gtk/app.h
    wx/gtk/assertdlg_gtk.h
    wx/gtk/bitmap.h
    wx/gtk/brush.h
    wx/gtk/clipbrd.h
    wx/gtk/colour.h
    wx/gtk/cursor.h
    wx/gtk/dataform.h
    wx/gtk/dataobj.h
    wx/gtk/dataobj2.h
    wx/gtk/dc.h
    wx/gtk/dcclient.h
    wx/gtk/dcmemory.h
    wx/gtk/dcscreen.h
    wx/gtk/dnd.h
    wx/gtk/evtloop.h
    wx/gtk/font.h
    wx/gtk/minifram.h
    wx/gtk/pen.h
    wx/gtk/popupwin.h
    wx/gtk/private/win_gtk.h
    wx/gtk/region.h
    wx/gtk/tooltip.h
    wx/gtk/toplevel.h
    wx/gtk/window.h
)
set(GTK_SRC
    ## Generic implementations used by wxGTK:
    src/generic/accel.cpp
    src/generic/fdrepdlg.cpp
    ## Needed as long as we support GTK+ < 2.6
    src/generic/filepickerg.cpp
    src/generic/graphicc.cpp
    src/generic/listctrl.cpp
    src/generic/prntdlgg.cpp
    src/generic/statusbr.cpp
    ## Generic implementations used by wxGPE:
    src/generic/fontdlgg.cpp
    src/generic/msgdlgg.cpp
    ## GTK+ specific files:
    src/gtk/artgtk.cpp
    src/gtk/bmpbuttn.cpp
    src/gtk/button.cpp
    src/gtk/checkbox.cpp
    src/gtk/checklst.cpp
    src/gtk/choice.cpp
    src/gtk/collpane.cpp
    src/gtk/colordlg.cpp
    src/gtk/combobox.cpp
    src/gtk/control.cpp
    src/gtk/clrpicker.cpp
    src/gtk/dialog.cpp
    src/gtk/fontpicker.cpp
    src/gtk/filepicker.cpp
    src/gtk/dirdlg.cpp
    src/gtk/filedlg.cpp
    src/gtk/fontdlg.cpp
    src/gtk/frame.cpp
    src/gtk/gauge.cpp
    src/gtk/gnome/gprint.cpp
    src/gtk/gnome/gvfs.cpp
    src/gtk/print.cpp
    src/gtk/listbox.cpp
    src/gtk/mdi.cpp
    src/gtk/menu.cpp
    src/gtk/mnemonics.cpp
    src/gtk/msgdlg.cpp
    src/gtk/nativewin.cpp
    src/gtk/notebook.cpp
    src/gtk/radiobox.cpp
    src/gtk/radiobut.cpp
    src/gtk/scrolbar.cpp
    src/gtk/scrolwin.cpp
    src/gtk/slider.cpp
    src/gtk/spinbutt.cpp
    src/gtk/spinctrl.cpp
    src/gtk/statbmp.cpp
    src/gtk/statbox.cpp
    src/gtk/statline.cpp
    src/gtk/stattext.cpp
    src/gtk/tbargtk.cpp
    src/gtk/textctrl.cpp
    src/gtk/textentry.cpp
    src/gtk/tglbtn.cpp
    src/gtk/treeentry_gtk.c
)
set(GTK_HDR
    wx/generic/fdrepdlg.h
    wx/generic/filepickerg.h
    wx/generic/fontdlgg.h
    wx/generic/listctrl.h
    wx/generic/statusbr.h
    wx/gtk/accel.h
    wx/gtk/bmpbuttn.h
    wx/gtk/button.h
    wx/gtk/checkbox.h
    wx/gtk/checklst.h
    wx/gtk/chkconf.h
    wx/gtk/collpane.h
    wx/gtk/colordlg.h
    wx/gtk/choice.h
    wx/gtk/combobox.h
    wx/gtk/control.h
    wx/gtk/clrpicker.h
    wx/gtk/dialog.h
    wx/gtk/dirdlg.h
    wx/gtk/filectrl.h
    wx/gtk/filedlg.h
    wx/gtk/fontpicker.h
    wx/gtk/filepicker.h
    wx/gtk/fontdlg.h
    wx/gtk/frame.h
    wx/gtk/gauge.h
    wx/gtk/gnome/gprint.h
    wx/gtk/gnome/gvfs.h
    wx/gtk/print.h
    wx/gtk/listbox.h
    wx/gtk/mdi.h
    wx/gtk/menu.h
    wx/gtk/menuitem.h
    wx/gtk/msgdlg.h
    wx/gtk/notebook.h
    wx/gtk/radiobox.h
    wx/gtk/radiobut.h
    wx/gtk/scrolbar.h
    wx/gtk/scrolwin.h
    wx/gtk/slider.h
    wx/gtk/spinbutt.h
    wx/gtk/spinctrl.h
    wx/gtk/statbmp.h
    wx/gtk/statbox.h
    wx/gtk/statline.h
    wx/gtk/stattext.h
    wx/gtk/tbargtk.h
    wx/gtk/textctrl.h
    wx/gtk/textentry.h
    wx/gtk/tglbtn.h
    wx/gtk/treeentry_gtk.h
)


## ======================================================================
##                                wxGTK1
## ======================================================================

set(GTK1_LOWLEVEL_SRC
    ${XWIN_LOWLEVEL_SRC}
    src/generic/icon.cpp
    src/generic/paletteg.cpp
    src/gtk1/app.cpp
    src/gtk1/bitmap.cpp
    src/gtk1/brush.cpp
    src/gtk1/clipbrd.cpp
    src/gtk1/colour.cpp
    src/gtk1/cursor.cpp
    src/gtk1/data.cpp
    src/gtk1/dataobj.cpp
    src/gtk1/dc.cpp
    src/gtk1/dcclient.cpp
    src/gtk1/dcmemory.cpp
    src/gtk1/dcscreen.cpp
    src/gtk1/dnd.cpp
    src/gtk1/evtloop.cpp
    src/gtk1/font.cpp
    src/gtk1/sockgtk.cpp
    src/gtk1/main.cpp
    src/gtk1/minifram.cpp
    src/gtk1/pen.cpp
    src/gtk1/popupwin.cpp
    src/gtk1/region.cpp
    src/gtk1/renderer.cpp
    src/gtk1/settings.cpp
    src/gtk1/timer.cpp
    src/gtk1/tooltip.cpp
    src/gtk1/toplevel.cpp
    src/gtk1/utilsgtk.cpp
    src/gtk1/win_gtk.c
    src/gtk1/window.cpp
)
set(GTK1_LOWLEVEL_HDR
    ${XWIN_LOWLEVEL_HDR}
    wx/generic/icon.h
    wx/generic/paletteg.h
    wx/gtk1/app.h
    wx/gtk1/bitmap.h
    wx/gtk1/brush.h
    wx/gtk1/clipbrd.h
    wx/gtk1/colour.h
    wx/gtk1/cursor.h
    wx/gtk1/dataform.h
    wx/gtk1/dataobj.h
    wx/gtk1/dataobj2.h
    wx/gtk1/dc.h
    wx/gtk1/dcclient.h
    wx/gtk1/dcmemory.h
    wx/gtk1/dcscreen.h
    wx/gtk1/dnd.h
    wx/gtk1/font.h
    wx/gtk1/minifram.h
    wx/gtk1/pen.h
    wx/gtk1/popupwin.h
    wx/gtk1/region.h
    wx/gtk1/tooltip.h
    wx/gtk1/toplevel.h
    wx/gtk1/win_gtk.h
    wx/gtk1/window.h
)
set(GTK1_SRC
    ## Generic implementations used by wxGTK1:
    src/generic/accel.cpp
    src/generic/clrpickerg.cpp
    src/generic/collpaneg.cpp
    src/generic/colrdlgg.cpp
    src/generic/dirdlgg.cpp
    src/generic/fdrepdlg.cpp
    src/generic/filedlgg.cpp
    src/generic/filepickerg.cpp
    src/generic/fontdlgg.cpp
    src/generic/fontpickerg.cpp
    src/generic/listctrl.cpp
    src/generic/msgdlgg.cpp
    src/generic/prntdlgg.cpp
    src/generic/statusbr.cpp
    ## GTK1 specific files:
    src/gtk1/bmpbuttn.cpp
    src/gtk1/button.cpp
    src/gtk1/checkbox.cpp
    src/gtk1/checklst.cpp
    src/gtk1/choice.cpp
    src/gtk1/combobox.cpp
    src/gtk1/control.cpp
    src/gtk1/dialog.cpp
    src/gtk1/filedlg.cpp
    src/gtk1/fontdlg.cpp
    src/gtk1/frame.cpp
    src/gtk1/gauge.cpp
    src/gtk1/listbox.cpp
    src/gtk1/mdi.cpp
    src/gtk1/menu.cpp
    src/gtk1/notebook.cpp
    src/gtk1/radiobox.cpp
    src/gtk1/radiobut.cpp
    src/gtk1/scrolbar.cpp
    src/gtk1/scrolwin.cpp
    src/gtk1/slider.cpp
    src/gtk1/spinbutt.cpp
    src/gtk1/spinctrl.cpp
    src/gtk1/statbmp.cpp
    src/gtk1/statbox.cpp
    src/gtk1/statline.cpp
    src/gtk1/stattext.cpp
    src/gtk1/tbargtk.cpp
    src/gtk1/textctrl.cpp
    src/gtk1/tglbtn.cpp
)
set(GTK1_HDR
    wx/generic/clrpickerg.h
    wx/generic/collpaneg.h
    wx/generic/colrdlgg.h
    wx/generic/dirdlgg.h
    wx/generic/fdrepdlg.h
    wx/generic/filedlgg.h
    wx/generic/filepickerg.h
    wx/generic/fontdlgg.h
    wx/generic/fontpickerg.h
    wx/generic/listctrl.h
    wx/generic/statusbr.h
    wx/gtk1/accel.h
    wx/gtk1/bmpbuttn.h
    wx/gtk1/button.h
    wx/gtk1/checkbox.h
    wx/gtk1/checklst.h
    wx/gtk1/choice.h
    wx/gtk1/combobox.h
    wx/gtk1/control.h
    wx/gtk1/dialog.h
    wx/gtk1/filedlg.h
    wx/gtk1/fontdlg.h
    wx/gtk1/frame.h
    wx/gtk1/gauge.h
    wx/gtk1/listbox.h
    wx/gtk1/mdi.h
    wx/gtk1/menu.h
    wx/gtk1/menuitem.h
    wx/gtk1/msgdlg.h
    wx/gtk1/notebook.h
    wx/gtk1/radiobox.h
    wx/gtk1/radiobut.h
    wx/gtk1/scrolbar.h
    wx/gtk1/scrolwin.h
    wx/gtk1/slider.h
    wx/gtk1/spinbutt.h
    wx/gtk1/spinctrl.h
    wx/gtk1/statbmp.h
    wx/gtk1/statbox.h
    wx/gtk1/statline.h
    wx/gtk1/stattext.h
    wx/gtk1/tbargtk.h
    wx/gtk1/textctrl.h
    wx/gtk1/tglbtn.h
    wx/gtk1/treectrl.h
)

## ======================================================================
##                               wxX11
## ======================================================================


set(X11_LOWLEVEL_SRC
    ${XWIN_LOWLEVEL_SRC}
    src/generic/icon.cpp
    src/generic/timer.cpp
    src/x11/app.cpp
    src/x11/bitmap.cpp
    src/x11/brush.cpp
    src/x11/clipbrd.cpp
    src/x11/colour.cpp
    src/x11/cursor.cpp
    src/x11/data.cpp
    src/x11/dataobj.cpp
    src/x11/dc.cpp
    src/x11/dcclient.cpp
    src/x11/dcmemory.cpp
    src/x11/dcscreen.cpp
    src/x11/evtloop.cpp
    src/x11/font.cpp
    src/x11/minifram.cpp
    src/x11/nanox.c
    src/x11/palette.cpp
    src/x11/pen.cpp
    src/x11/popupwin.cpp
    src/x11/region.cpp
    src/x11/reparent.cpp
    src/x11/settings.cpp
    src/x11/toplevel.cpp
    src/x11/utils.cpp
    src/x11/utilsx.cpp
    src/x11/window.cpp
)
set(X11_LOWLEVEL_HDR
    ${XWIN_LOWLEVEL_HDR}
    wx/generic/icon.h
    wx/x11/app.h
    wx/x11/bitmap.h
    wx/x11/brush.h
    wx/x11/chkconf.h
    wx/x11/clipbrd.h
    wx/x11/colour.h
    wx/x11/cursor.h
    wx/x11/dataform.h
    wx/x11/dataobj.h
    wx/x11/dataobj2.h
    wx/x11/dc.h
    wx/x11/dcclient.h
    wx/x11/dcmemory.h
    wx/x11/dcprint.h
    wx/x11/dcscreen.h
    wx/x11/dnd.h
    wx/x11/font.h
    wx/x11/minifram.h
    wx/x11/palette.h
    wx/x11/pen.h
    wx/x11/popupwin.h
    wx/x11/print.h
    wx/x11/private.h
    wx/x11/privx.h
    wx/x11/region.h
    wx/x11/reparent.h
    wx/x11/textctrl.h
    wx/x11/toplevel.h
    wx/x11/window.h
)



## ======================================================================
##                                wxMSW
## ======================================================================

set(MSW_LOWLEVEL_SRC
    src/msw/app.cpp
    src/msw/bitmap.cpp
    src/msw/brush.cpp
    src/msw/caret.cpp
    src/msw/clipbrd.cpp
    src/msw/colour.cpp
    src/msw/cursor.cpp
    src/msw/data.cpp
    src/msw/dc.cpp
    src/msw/dcclient.cpp
    src/msw/dcmemory.cpp
    src/msw/dcprint.cpp
    src/msw/dcscreen.cpp
    src/msw/dialup.cpp
    src/msw/dib.cpp
    src/msw/display.cpp
    src/msw/enhmeta.cpp
    src/msw/font.cpp
    src/msw/fontenum.cpp
    src/msw/fontutil.cpp
    src/msw/gdiimage.cpp
    src/msw/gdiobj.cpp
    src/msw/gdiplus.cpp
    src/msw/graphics.cpp
    src/msw/icon.cpp
    src/msw/imaglist.cpp
    src/msw/minifram.cpp
    src/msw/ole/dataobj.cpp
    src/msw/ole/dropsrc.cpp
    src/msw/ole/droptgt.cpp
    src/msw/ole/oleutils.cpp
    src/msw/palette.cpp
    src/msw/pen.cpp
    src/msw/popupwin.cpp
    src/msw/printdlg.cpp
    src/msw/printwin.cpp
    src/msw/region.cpp
    src/msw/renderer.cpp
    src/msw/settings.cpp
    src/msw/tooltip.cpp
    src/msw/toplevel.cpp
    src/msw/utilsgui.cpp
    src/msw/uxtheme.cpp
    src/msw/window.cpp
)
set(MSW_LOWLEVEL_HDR
    wx/msw/popupwin.h
    wx/msw/uxtheme.h
    wx/msw/uxthemep.h
    wx/msw/htmlhelp.h
)

set(MSW_DESKTOP_LOWLEVEL_SRC
    src/msw/helpchm.cpp
    src/msw/helpwin.cpp
    src/msw/ole/automtn.cpp
    src/msw/ole/uuid.cpp
)

set(MSW_DESKTOP_LOWLEVEL_HDR
    wx/msw/helpchm.h
    wx/msw/helpwin.h
)

set(MSW_SRC
    src/generic/clrpickerg.cpp
    src/generic/collpaneg.cpp
    src/generic/filepickerg.cpp
    src/generic/fontpickerg.cpp
    src/generic/statusbr.cpp
    src/generic/prntdlgg.cpp
    src/msw/accel.cpp
    src/msw/artmsw.cpp
    src/msw/bmpbuttn.cpp
    src/msw/button.cpp
    src/msw/checkbox.cpp
    src/msw/choice.cpp
    src/msw/colordlg.cpp
    src/msw/combo.cpp
    src/msw/combobox.cpp
    src/msw/control.cpp
    src/msw/dialog.cpp
    src/msw/dirdlg.cpp
    src/msw/dragimag.cpp
    src/msw/filedlg.cpp
    src/msw/frame.cpp
    src/msw/gauge.cpp
    src/msw/headerctrl.cpp
    src/msw/iniconf.cpp
    src/msw/listbox.cpp
    src/msw/listctrl.cpp
    src/msw/mdi.cpp
    src/msw/menu.cpp
    src/msw/menuitem.cpp
    src/msw/metafile.cpp
    src/msw/msgdlg.cpp
    src/msw/nativdlg.cpp
    src/msw/nativewin.cpp
    src/msw/notebook.cpp
    src/msw/ole/access.cpp
    src/msw/ole/activex.cpp
    src/msw/ownerdrw.cpp
    src/msw/radiobox.cpp
    src/msw/radiobut.cpp
    src/msw/scrolbar.cpp
    src/msw/slider.cpp
    src/msw/spinbutt.cpp
    src/msw/spinctrl.cpp
    src/msw/statbmp.cpp
    src/msw/statbox.cpp
    src/msw/statusbar.cpp
    src/msw/statline.cpp
    src/msw/stattext.cpp
    src/msw/toolbar.cpp
    src/msw/textctrl.cpp
    src/msw/textentry.cpp
    src/msw/tglbtn.cpp
    src/msw/treectrl.cpp
)
set(MSW_HDR
    wx/generic/clrpickerg.h
    wx/generic/collpaneg.h
    wx/generic/filepickerg.h
    wx/generic/fontpickerg.h
    wx/msw/accel.h
    wx/msw/app.h
    wx/msw/bitmap.h
    wx/msw/bmpbuttn.h
    wx/msw/brush.h
    wx/msw/button.h
    wx/msw/caret.h
    wx/msw/checkbox.h
    wx/msw/choice.h
    wx/msw/clipbrd.h
    wx/msw/colordlg.h
    wx/msw/colour.h
    wx/msw/combo.h
    wx/msw/combobox.h
    wx/msw/control.h
    wx/msw/ctrlsub.h
    wx/msw/cursor.h
    wx/msw/dc.h
    wx/msw/dcclient.h
    wx/msw/dcmemory.h
    wx/msw/dcprint.h
    wx/msw/dcscreen.h
    wx/msw/dialog.h
    wx/msw/dib.h
    wx/msw/dirdlg.h
    wx/msw/dragimag.h
    wx/msw/enhmeta.h
    wx/msw/evtloop.h
    wx/msw/filedlg.h
    wx/msw/font.h
    wx/msw/frame.h
    wx/msw/gauge.h
    wx/msw/gdiimage.h
    wx/msw/headerctrl.h
    wx/msw/icon.h
    wx/msw/imaglist.h
    wx/msw/iniconf.h
    wx/msw/listbox.h
    wx/msw/listctrl.h
    wx/msw/mdi.h
    wx/msw/menu.h
    wx/msw/menuitem.h
    wx/msw/metafile.h
    wx/msw/minifram.h
    wx/msw/missing.h
    wx/msw/msgdlg.h
    wx/msw/msvcrt.h
    wx/msw/notebook.h
    wx/msw/ole/access.h
    wx/msw/ole/activex.h
    wx/msw/ole/dataform.h
    wx/msw/ole/dataobj.h
    wx/msw/ole/dataobj2.h
    wx/msw/ole/dropsrc.h
    wx/msw/ole/droptgt.h
    wx/msw/ole/oleutils.h
    wx/msw/palette.h
    wx/msw/pen.h
    wx/msw/printdlg.h
    wx/msw/printwin.h
    wx/msw/radiobox.h
    wx/msw/radiobut.h
    wx/msw/region.h
    wx/msw/rcdefs.h
    wx/msw/scrolbar.h
    wx/msw/setup0.h
    wx/msw/slider.h
    wx/msw/spinbutt.h
    wx/msw/spinctrl.h
    wx/msw/statbmp.h
    wx/msw/statbox.h
    wx/msw/statusbar.h
    wx/msw/statline.h
    wx/msw/stattext.h
    wx/msw/toolbar.h
    wx/msw/textctrl.h
    wx/msw/textentry.h
    wx/msw/tglbtn.h
    wx/msw/tooltip.h
    wx/msw/toplevel.h
    wx/msw/treectrl.h
    wx/msw/window.h

    ## Resources must be installed together with headers:
    wx/msw/wx.manifest
    wx/msw/amd64.manifest
    wx/msw/ia64.manifest
    wx/msw/wx.rc
    ## bitmaps
    wx/msw/colours.bmp
    wx/msw/csquery.bmp
    ## cursors
    wx/msw/blank.cur
    wx/msw/bullseye.cur
    wx/msw/cross.cur
    wx/msw/hand.cur
    wx/msw/magnif1.cur
    wx/msw/pbrush.cur
    wx/msw/pencil.cur
    wx/msw/pntleft.cur
    wx/msw/pntright.cur
    wx/msw/rightarr.cur
    wx/msw/roller.cur
    ## icons
    wx/msw/cdrom.ico
    wx/msw/child.ico
    wx/msw/computer.ico
    wx/msw/drive.ico
    wx/msw/file1.ico
    wx/msw/floppy.ico
    wx/msw/folder1.ico
    wx/msw/folder2.ico
    wx/msw/mdi.ico
    wx/msw/question.ico
    wx/msw/removble.ico
    wx/msw/std.ico
)

## Files used only by desktop MSW port, but *not* WindowsCE one:
set(MSW_DESKTOP_SRC
    src/msw/checklst.cpp
    src/msw/fdrepdlg.cpp
    src/msw/fontdlg.cpp
)
set(MSW_DESKTOP_HDR
    wx/msw/checklst.h
    wx/msw/fdrepdlg.h
    wx/msw/fontdlg.h
    wx/msw/ole/automtn.h
    wx/msw/ole/uuid.h
)

## Files used by WindowsCE port but not by desktop MSW port:
set(WINCE_SRC
    src/generic/dirdlgg.cpp
    src/generic/fdrepdlg.cpp
    src/generic/filedlgg.cpp
    src/generic/fontdlgg.cpp
    src/msw/wince/checklst.cpp
    src/msw/wince/choicece.cpp
    src/msw/wince/crt.cpp
    src/msw/wince/filedlgwce.cpp
    src/msw/wince/filefnwce.cpp
    src/msw/wince/helpwce.cpp
    src/msw/wince/menuce.cpp
    src/msw/wince/tbarwce.cpp
    src/msw/wince/textctrlce.cpp
)
set(WINCE_HDR
    wx/generic/fdrepdlg.h
    wx/generic/filedlgg.h
    wx/generic/fontdlgg.h
    wx/msw/wince/checklst.h
    wx/msw/wince/choicece.h
    wx/msw/wince/helpwce.h
    wx/msw/wince/libraries.h
    wx/msw/wince/missing.h
    wx/msw/wince/tbarwce.h
    wx/msw/wince/textctrlce.h
    wx/msw/wince/resources.h
)


## ======================================================================
##                                wxMGL
## ======================================================================

set(MGL_LOWLEVEL_SRC
    src/common/fontmgrcmn.cpp
    src/generic/caret.cpp
    src/generic/colour.cpp
    src/generic/icon.cpp
    src/generic/imaglist.cpp
    src/generic/mask.cpp
    src/generic/timer.cpp
    src/mgl/app.cpp
    src/mgl/bitmap.cpp
    src/mgl/brush.cpp
    src/mgl/clipbrd.cpp
    src/mgl/cursor.cpp
    src/mgl/data.cpp
    src/mgl/dc.cpp
    src/mgl/dcclient.cpp
    src/mgl/dcmemory.cpp
    src/mgl/dcscreen.cpp
    src/mgl/dirmgl.cpp
    src/mgl/evtloop.cpp
    src/mgl/font.cpp
    src/mgl/fontenum.cpp
    src/mgl/fontmgr.cpp
    src/mgl/fontutil.cpp
    src/mgl/palette.cpp
    src/mgl/pen.cpp
    src/mgl/region.cpp
    src/mgl/settings.cpp
    src/mgl/toplevel.cpp
    src/mgl/utils.cpp
    src/mgl/window.cpp
)
set(MGL_LOWLEVEL_HDR
    wx/generic/caret.h
    wx/generic/colour.h
    wx/generic/icon.h
    wx/generic/imaglist.h
    wx/generic/mask.h
    wx/mgl/app.h
    wx/mgl/bitmap.h
    wx/mgl/brush.h
    wx/mgl/chkconf.h
    wx/mgl/clipbrd.h
    wx/mgl/cursor.h
    wx/mgl/dc.h
    wx/mgl/dcclient.h
    wx/mgl/dcmemory.h
    wx/mgl/dcscreen.h
    wx/mgl/font.h
    wx/mgl/palette.h
    wx/mgl/pen.h
    wx/mgl/popupwin.h
    wx/mgl/private.h
    wx/mgl/region.h
    wx/mgl/toplevel.h
    wx/mgl/window.h
)


## ======================================================================
##                                wxDFB
## ======================================================================

set(DFB_LOWLEVEL_SRC
    src/common/fontmgrcmn.cpp
    src/generic/caret.cpp
    src/generic/colour.cpp
    src/generic/icon.cpp
    src/generic/imaglist.cpp
    src/generic/mask.cpp
    src/dfb/app.cpp
    src/dfb/bitmap.cpp
    src/dfb/brush.cpp
    src/dfb/cursor.cpp
    ##    src/dfb/data.cpp
    src/dfb/dc.cpp
    src/dfb/dcclient.cpp
    src/dfb/dcmemory.cpp
    src/dfb/dcscreen.cpp
    src/dfb/evtloop.cpp
    src/dfb/font.cpp
    src/dfb/fontenum.cpp
    src/dfb/fontmgr.cpp
    src/dfb/nonownedwnd.cpp
    src/dfb/overlay.cpp
    src/dfb/pen.cpp
    src/dfb/region.cpp
    src/dfb/settings.cpp
    src/dfb/toplevel.cpp
    src/dfb/utils.cpp
    src/dfb/window.cpp
    src/dfb/wrapdfb.cpp
)
set(DFB_LOWLEVEL_HDR
    wx/generic/caret.h
    wx/generic/colour.h
    wx/generic/icon.h
    wx/generic/imaglist.h
    wx/generic/mask.h
    wx/dfb/app.h
    wx/dfb/bitmap.h
    wx/dfb/brush.h
    wx/dfb/chkconf.h
    wx/dfb/cursor.h
    wx/dfb/dc.h
    wx/dfb/dcclient.h
    wx/dfb/dcmemory.h
    wx/dfb/dcscreen.h
    wx/dfb/dfbptr.h
    wx/dfb/evtloop.h
    wx/dfb/font.h
    wx/dfb/nonownedwnd.h
    wx/dfb/pen.h
    wx/dfb/popupwin.h
    wx/dfb/private.h
    wx/dfb/region.h
    wx/dfb/toplevel.h
    wx/dfb/window.h
    wx/dfb/wrapdfb.h
)


## ======================================================================
##                               wxOS/2
## ======================================================================

set(OS2_LOWLEVEL_SRC
    ## wxUniv builds not supported under OS/2
)
set(OS2_LOWLEVEL_HDR
)
set(OS2_SRC
    ## Generic implementations used by wxOS2:
    src/generic/caret.cpp
    src/generic/clrpickerg.cpp
    src/generic/collpaneg.cpp
    src/generic/colrdlgg.cpp
    src/generic/dirdlgg.cpp
    src/generic/fdrepdlg.cpp
    src/generic/filepickerg.cpp
    src/generic/fontpickerg.cpp
    src/generic/imaglist.cpp
    src/generic/listctrl.cpp
    src/generic/mdig.cpp
    src/generic/prntdlgg.cpp
    src/generic/statusbr.cpp
    ## OS/2 specific files:
    src/os2/accel.cpp
    src/os2/app.cpp
    src/os2/bitmap.cpp
    src/os2/bmpbuttn.cpp
    src/os2/brush.cpp
    src/os2/button.cpp
    src/os2/checkbox.cpp
    src/os2/checklst.cpp
    src/os2/choice.cpp
    src/os2/clipbrd.cpp
    src/os2/colour.cpp
    src/os2/combobox.cpp
    src/os2/control.cpp
    src/os2/cursor.cpp
    src/os2/data.cpp
    src/os2/dataobj.cpp
    src/os2/dc.cpp
    src/os2/dcclient.cpp
    src/os2/dcmemory.cpp
    src/os2/dcprint.cpp
    src/os2/dcscreen.cpp
    src/os2/dialog.cpp
    src/os2/dnd.cpp
    src/os2/evtloop.cpp
    src/os2/filedlg.cpp
    src/os2/font.cpp
    src/os2/fontdlg.cpp
    src/os2/fontenum.cpp
    src/os2/fontutil.cpp
    src/os2/frame.cpp
    src/os2/gauge.cpp
    src/os2/gdiimage.cpp
    src/os2/sockpm.cpp
    src/os2/helpwin.cpp
    src/os2/icon.cpp
    src/os2/iniconf.cpp
    src/os2/listbox.cpp
    src/os2/main.cpp
    src/os2/menu.cpp
    src/os2/menuitem.cpp
    src/os2/metafile.cpp
    src/os2/minifram.cpp
    src/os2/msgdlg.cpp
    src/os2/nativdlg.cpp
    src/os2/notebook.cpp
    src/os2/ownerdrw.cpp
    src/os2/palette.cpp
    src/os2/pen.cpp
    src/os2/popupwin.cpp
    src/os2/print.cpp
    src/os2/radiobox.cpp
    src/os2/radiobut.cpp
    src/os2/region.cpp
    src/os2/scrolbar.cpp
    src/os2/settings.cpp
    src/os2/slider.cpp
    src/os2/spinbutt.cpp
    src/os2/spinctrl.cpp
    src/os2/statbmp.cpp
    src/os2/statbox.cpp
    src/os2/statline.cpp
    src/os2/stattext.cpp
    src/os2/textctrl.cpp
    src/os2/textentry.cpp
    src/os2/tglbtn.cpp
    src/os2/timer.cpp
    src/os2/toolbar.cpp
    src/os2/tooltip.cpp
    src/os2/toplevel.cpp
    src/os2/utilsgui.cpp
    src/os2/window.cpp
)
set(OS2_HDR
    wx/generic/caret.h
    wx/generic/clrpickerg.h
    wx/generic/collpaneg.h
    wx/generic/colrdlgg.h
    wx/generic/dirdlgg.h
    wx/generic/fdrepdlg.h
    wx/generic/listctrl.h
    wx/generic/mdig.h
    wx/generic/statusbr.h
    wx/os2/accel.h
    wx/os2/app.h
    wx/os2/bitmap.h
    wx/os2/bmpbuttn.h
    wx/os2/brush.h
    wx/os2/button.h
    wx/os2/checkbox.h
    wx/os2/checklst.h
    wx/os2/choice.h
    wx/os2/clipbrd.h
    wx/os2/colour.h
    wx/os2/combobox.h
    wx/os2/control.h
    wx/os2/cursor.h
    wx/os2/dataform.h
    wx/os2/dataobj.h
    wx/os2/dataobj2.h
    wx/os2/dc.h
    wx/os2/dcclient.h
    wx/os2/dcmemory.h
    wx/os2/dcprint.h
    wx/os2/dcscreen.h
    wx/os2/dialog.h
    wx/os2/dnd.h
    wx/os2/filedlg.h
    wx/os2/font.h
    wx/os2/fontdlg.h
    wx/os2/frame.h
    wx/os2/gauge.h
    wx/os2/gdiimage.h
    wx/os2/helpwin.h
    wx/os2/icon.h
    wx/os2/iniconf.h
    wx/os2/listbox.h
    wx/os2/menu.h
    wx/os2/menuitem.h
    wx/os2/metafile.h
    wx/os2/minifram.h
    wx/os2/msgdlg.h
    wx/os2/notebook.h
    wx/os2/palette.h
    wx/os2/pen.h
    wx/os2/pnghand.h
    wx/os2/pngread.h
    wx/os2/print.h
    wx/os2/radiobox.h
    wx/os2/radiobut.h
    wx/os2/region.h
    wx/os2/scrolbar.h
    wx/os2/settings.h
    wx/os2/setup0.h
    wx/os2/slider.h
    wx/os2/spinbutt.h
    wx/os2/spinctrl.h
    wx/os2/statbmp.h
    wx/os2/statbox.h
    wx/os2/statline.h
    wx/os2/stattext.h
    wx/os2/textctrl.h
    wx/os2/textentry.h
    wx/os2/tglbtn.h
    wx/os2/toolbar.h
    wx/os2/tooltip.h
    wx/os2/toplevel.h
    wx/os2/window.h
)



## ======================================================================
##                                wxMac
## ======================================================================

set(OSX_LOWLEVEL_SRC
    ## Shared wxMac and wxCocoa files
    src/osx/accel.cpp
    src/osx/artmac.cpp
    src/osx/bmpbuttn_osx.cpp
    src/osx/brush.cpp
    src/osx/button_osx.cpp
    src/osx/checkbox_osx.cpp
    src/osx/checklst_osx.cpp
    src/osx/choice_osx.cpp
    src/osx/dialog_osx.cpp
    src/osx/fontutil.cpp
    src/osx/gauge_osx.cpp
    src/osx/imaglist.cpp
    src/osx/listbox_osx.cpp
    src/osx/menu_osx.cpp
    src/osx/menuitem_osx.cpp
    src/osx/minifram.cpp
    src/osx/nonownedwnd_osx.cpp
    src/osx/notebook_osx.cpp
    src/osx/palette.cpp
    src/osx/pen.cpp
    src/osx/printdlg_osx.cpp
    src/osx/radiobox_osx.cpp
    src/osx/radiobut_osx.cpp
    src/osx/scrolbar_osx.cpp
    src/osx/slider_osx.cpp
    src/osx/spinbutt_osx.cpp
    src/osx/spinctrl_osx.cpp
    src/osx/srchctrl_osx.cpp
    src/osx/statbox_osx.cpp
    src/osx/statline_osx.cpp
    src/osx/stattext_osx.cpp
    src/osx/textctrl_osx.cpp
    src/osx/tglbtn_osx.cpp
    src/osx/toolbar_osx.cpp
    src/osx/toplevel_osx.cpp
    src/osx/utils_osx.cpp
    ## src/osx/treectrl.cpp
    src/osx/window_osx.cpp

    src/osx/core/bitmap.cpp
    src/osx/core/colour.cpp
    src/osx/core/dcmemory.cpp
    src/osx/core/display.cpp
    src/osx/core/fontenum.cpp
    src/osx/core/hid.cpp
    src/osx/core/printmac.cpp
    src/osx/core/timer.cpp
    src/osx/core/utilsexc_cf.cpp
)

set(OSX_LOWLEVEL_HDR
)


set(OSX_CARBON_COCOA_SRC
    ## wxWebKit files
    src/html/htmlctrl/webkit/webkit.mm
    ## Native color/font dialogs
    src/osx/carbon/colordlgosx.mm
    src/osx/carbon/fontdlgosx.mm
    ## other shared files
    src/osx/carbon/clipbrd.cpp
    src/osx/carbon/cursor.cpp
    src/osx/carbon/fontdlg.cpp
    src/osx/carbon/gdiobj.cpp
    src/osx/carbon/icon.cpp
    src/osx/carbon/app.cpp
    src/osx/carbon/combobox.cpp
    src/osx/carbon/control.cpp
    src/osx/carbon/dataobj.cpp
    src/osx/carbon/dcclient.cpp
    src/osx/carbon/dcprint.cpp
    src/osx/carbon/dcscreen.cpp
    src/osx/core/glgrab.cpp
    src/osx/carbon/graphics.cpp
    src/osx/carbon/dnd.cpp
    src/osx/carbon/font.cpp
    src/osx/carbon/frame.cpp
    src/osx/carbon/mdi.cpp
    src/osx/carbon/metafile.cpp
    src/osx/carbon/overlay.cpp
    src/osx/carbon/popupwin.cpp
    src/osx/carbon/renderer.cpp
    src/osx/carbon/settings.cpp
    src/osx/carbon/statbrma.cpp
    src/osx/carbon/region.cpp
    ## cocoa bridge
    src/osx/carbon/utilscocoa.mm
    ## Generic implementations used by wxOSX:
    src/generic/caret.cpp
    src/generic/clrpickerg.cpp
    src/generic/collpaneg.cpp
    src/generic/colrdlgg.cpp
    src/generic/dirdlgg.cpp
    src/generic/fdrepdlg.cpp
    src/generic/filedlgg.cpp
    src/generic/filepickerg.cpp
    src/generic/fontdlgg.cpp
    src/generic/fontpickerg.cpp
    src/generic/listctrl.cpp
    src/generic/prntdlgg.cpp
    src/generic/statusbr.cpp
)

set(OSX_CARBON_SRC
    ## carbon files
    ${OSX_CARBON_COCOA_SRC}
    src/osx/carbon/bmpbuttn.cpp
    src/osx/carbon/button.cpp
    src/osx/carbon/checkbox.cpp
    src/osx/carbon/choice.cpp
    src/osx/carbon/colordlg.cpp
    src/osx/carbon/dialog.cpp
    src/osx/carbon/dirdlg.cpp
    src/osx/carbon/evtloop.cpp
    src/osx/carbon/filedlg.cpp
    src/osx/carbon/gauge.cpp
    src/osx/carbon/listbox.cpp
    src/osx/carbon/listctrl_mac.cpp
    src/osx/carbon/menu.cpp
    src/osx/carbon/menuitem.cpp
    src/osx/carbon/msgdlg.cpp
    src/osx/carbon/nonownedwnd.cpp
    src/osx/carbon/notebmac.cpp
    src/osx/carbon/printdlg.cpp
    src/osx/carbon/radiobut.cpp
    src/osx/carbon/scrolbar.cpp
    src/osx/carbon/slider.cpp
    src/osx/carbon/spinbutt.cpp
    src/osx/carbon/srchctrl.cpp
    src/osx/carbon/statbmp.cpp
    src/osx/carbon/statbox.cpp
    src/osx/carbon/statlmac.cpp
    src/osx/carbon/stattext.cpp
    src/osx/carbon/textctrl.cpp
    src/osx/carbon/tglbtn.cpp
    src/osx/carbon/toolbar.cpp
    src/osx/carbon/tooltip.cpp
    src/osx/carbon/window.cpp
)

## Header files like wx/osx/foo.h which include wx/osx/carbon/foo.h
set(OSX_SHARED_HDR
    ## wxWebKit headers
    wx/html/webkit.h
    ## other shared headers
    wx/osx/accel.h
    wx/osx/app.h
    wx/osx/bitmap.h
    wx/osx/bmpbuttn.h
    wx/osx/brush.h
    wx/osx/button.h
    wx/osx/checkbox.h
    wx/osx/checklst.h
    wx/osx/chkconf.h
    wx/osx/choice.h
    wx/osx/clipbrd.h
    wx/osx/colordlg.h
    wx/osx/colour.h
    wx/osx/combobox.h
    wx/osx/control.h
    wx/osx/cursor.h
    wx/osx/dataform.h
    wx/osx/dataobj.h
    wx/osx/dataobj2.h
    wx/osx/dc.h
    wx/osx/dcclient.h
    wx/osx/dcmemory.h
    wx/osx/dcprint.h
    wx/osx/dcscreen.h
    wx/osx/dialog.h
    wx/osx/dirdlg.h
    wx/osx/dnd.h
    wx/osx/evtloop.h
    wx/osx/filedlg.h
    wx/osx/font.h
    wx/osx/fontdlg.h
    wx/osx/frame.h
    wx/osx/gauge.h
    wx/osx/helpxxxx.h
    wx/osx/icon.h
    wx/osx/imaglist.h
    wx/osx/listbox.h
    wx/osx/listctrl.h
    wx/osx/mdi.h
    wx/osx/menu.h
    wx/osx/menuitem.h
    wx/osx/metafile.h
    wx/osx/mimetype.h
    wx/osx/minifram.h
    wx/osx/msgdlg.h
    wx/osx/nonownedwnd.h
    wx/osx/notebook.h
    wx/osx/palette.h
    wx/osx/pen.h
    wx/osx/popupwin.h
    wx/osx/pnghand.h
    wx/osx/pngread.h
    wx/osx/printdlg.h
    wx/osx/printmac.h
    wx/osx/private.h
    wx/osx/radiobox.h
    wx/osx/radiobut.h
    wx/osx/region.h
    wx/osx/scrolbar.h
    wx/osx/slider.h
    wx/osx/spinbutt.h
    wx/osx/spinctrl.h
    wx/osx/srchctrl.h
    wx/osx/statbmp.h
    wx/osx/statbox.h
    wx/osx/statline.h
    wx/osx/stattext.h
    wx/osx/statusbr.h
    wx/osx/taskbarosx.h
    wx/osx/textctrl.h
    wx/osx/tglbtn.h
    wx/osx/toolbar.h
    wx/osx/tooltip.h
    wx/osx/toplevel.h
    wx/osx/treectrl.h
    wx/osx/uma.h
    wx/osx/window.h
    ## Generic implementations used by wxOSX:
    wx/generic/caret.h
    wx/generic/clrpickerg.h
    wx/generic/collpaneg.h
    wx/generic/colrdlgg.h
    wx/generic/dirdlgg.h
    wx/generic/fdrepdlg.h
    wx/generic/filedlgg.h
    wx/generic/filepickerg.h
    wx/generic/fontdlgg.h
    wx/generic/fontpickerg.h
    wx/generic/listctrl.h
    wx/generic/prntdlgg.h
    wx/generic/statusbr.h
)
## wxMac Carbon header files
set(OSX_CARBON_HDR
    ## core files
    wx/osx/core/colour.h
    ## carbon files
    wx/osx/carbon/chkconf.h
    wx/osx/carbon/evtloop.h
    wx/osx/carbon/private.h
    wx/osx/carbon/private/mactext.h
    wx/osx/carbon/private/overlay.h
    wx/osx/carbon/private/print.h
    wx/osx/carbon/region.h
    wx/osx/carbon/statbmp.h
    wx/osx/carbon/uma.h
)

## ======================================================================
##                              wxMac/Cocoa
## ======================================================================

set(OSX_COCOA_SRC
    ${OSX_CARBON_COCOA_SRC}
    src/osx/cocoa/bmpbuttn.mm
    src/osx/cocoa/button.mm
    src/osx/cocoa/checkbox.mm
    src/osx/cocoa/choice.mm
    src/osx/cocoa/dialog.mm
    src/osx/cocoa/dirdlg.mm
    src/osx/cocoa/evtloop.mm
    src/osx/cocoa/filedlg.mm
    src/osx/cocoa/gauge.mm
    src/osx/cocoa/listbox.mm
    src/osx/cocoa/menu.mm
    src/osx/cocoa/menuitem.mm
    src/osx/cocoa/msgdlg.mm
    src/osx/cocoa/nonownedwnd.mm
    src/osx/cocoa/notebook.mm
    src/osx/cocoa/radiobut.mm
    src/osx/cocoa/printdlg.mm
    src/osx/cocoa/scrolbar.mm
    src/osx/cocoa/slider.mm
    src/osx/cocoa/spinbutt.mm
    src/osx/cocoa/srchctrl.mm
    src/osx/cocoa/statbox.mm
    src/osx/cocoa/statline.mm
    src/osx/cocoa/stattext.mm
    src/osx/cocoa/textctrl.mm
    src/osx/cocoa/tglbtn.mm
    src/osx/cocoa/toolbar.mm
    src/osx/cocoa/tooltip.mm
    src/osx/cocoa/window.mm
)
set(OSX_COCOA_HDR
    wx/osx/cocoa/chkconf.h
    wx/osx/cocoa/evtloop.h
    wx/osx/cocoa/private.h
    ${OSX_CARBON_HDR} ## FIXME: these headers need moved somewhere shared...
    wx/generic/region.h
)

## ======================================================================
##                              wxMac/iPhone
## ======================================================================

set(OSX_IPHONE_SRC
src/osx/iphone/nonownedwnd.mm
src/osx/iphone/utils.mm
src/osx/iphone/window.mm
)

## ======================================================================
##                              wxCocoa
## ======================================================================

set(COCOA_LOWLEVEL_SRC
    ## Shared wxMac and wxCocoa files
    src/osx/core/hid.cpp
    src/osx/core/utilsexc_cf.cpp
)
set(COCOA_LOWLEVEL_HDR
)
set(COCOA_SRC
    src/cocoa/NSBox.mm
    src/cocoa/NSButton.mm
    src/cocoa/NSControl.mm
    src/cocoa/NSMenu.mm
    src/cocoa/NSPanel.mm
    src/cocoa/NSScroller.mm
    src/cocoa/NSSlider.mm
    src/cocoa/NSTabView.mm
    src/cocoa/NSTableView.mm
    src/cocoa/NSTextField.mm
    src/cocoa/NSView.mm
    src/cocoa/NSWindow.mm
    src/cocoa/ObjcRef.mm
    src/cocoa/app.mm
    src/cocoa/bitmap.mm
    src/cocoa/bmpbuttn.mm
    src/cocoa/brush.mm
    src/cocoa/button.mm
    src/cocoa/checkbox.mm
    src/cocoa/checklst.mm
    src/cocoa/choice.mm
    src/cocoa/clipbrd.mm
    src/cocoa/colordlg.mm
    src/cocoa/colour.mm
    src/cocoa/combobox.mm
    src/cocoa/control.mm
    src/cocoa/cursor.mm
    src/cocoa/data.cpp
    src/cocoa/dataobj.mm
    src/cocoa/dc.mm
    src/cocoa/dcclient.mm
    src/cocoa/dcmemory.mm
    src/cocoa/dcscreen.cpp
    src/cocoa/dialog.mm
    src/cocoa/dirdlg.mm
    src/cocoa/display.mm
    src/cocoa/drawer.mm
    src/cocoa/evtloop.mm
    src/cocoa/filedlg.mm
    src/cocoa/font.mm
    src/cocoa/fontdlg.mm
    src/cocoa/fontenum.mm
    src/cocoa/fontutil.cpp
    src/cocoa/frame.mm
    src/cocoa/gauge.mm
    src/cocoa/icon.mm
    src/cocoa/listbox.mm
    src/cocoa/main.cpp
    src/cocoa/mbarman.mm
    src/cocoa/mdi.mm
    src/cocoa/menu.mm
    src/cocoa/menuitem.mm
    src/cocoa/msgdlg.mm
    src/cocoa/notebook.mm
    src/cocoa/pen.mm
    src/cocoa/radiobox.mm
    src/cocoa/radiobut.mm
    src/cocoa/region.mm
    src/cocoa/scrolbar.mm
    src/cocoa/settings.mm
    src/cocoa/slider.mm
    src/cocoa/sound.mm
    src/cocoa/spinbutt.mm
    src/cocoa/statbmp.mm
    src/cocoa/statbox.mm
    src/cocoa/statline2.mm
    src/cocoa/stattext.mm
    src/cocoa/textctrl.mm
    src/cocoa/timer.mm
    src/cocoa/toolbar.mm
    src/cocoa/tooltip.mm
    src/cocoa/toplevel.mm
    src/cocoa/utils.mm
    src/cocoa/utilsexc.mm
    src/cocoa/window.mm
    ## Generic implementations used by wxCocoa:
    src/generic/accel.cpp
    src/generic/caret.cpp
    src/generic/clrpickerg.cpp
    src/generic/collpaneg.cpp
    src/generic/colrdlgg.cpp
    src/generic/fdrepdlg.cpp
    src/generic/filepickerg.cpp
    src/generic/fontdlgg.cpp
    src/generic/fontpickerg.cpp
    src/generic/imaglist.cpp
    src/generic/listctrl.cpp
    src/generic/msgdlgg.cpp
    src/generic/paletteg.cpp
    src/generic/regiong.cpp
    src/generic/statusbr.cpp
)
set(COCOA_HDR
    wx/cocoa/NSApplication.h
    wx/cocoa/NSBox.h
    wx/cocoa/NSButton.h
    wx/cocoa/NSControl.h
    wx/cocoa/NSMenu.h
    wx/cocoa/NSPanel.h
    wx/cocoa/NSScroller.h
    wx/cocoa/NSSlider.h
    wx/cocoa/NSTabView.h
    wx/cocoa/NSTableView.h
    wx/cocoa/NSTextField.h
    wx/cocoa/NSView.h
    wx/cocoa/NSWindow.h
    wx/cocoa/ObjcAssociate.h
    wx/cocoa/ObjcPose.h
    wx/cocoa/ObjcRef.h
    wx/cocoa/app.h
    wx/cocoa/bitmap.h
    wx/cocoa/bmpbuttn.h
    wx/cocoa/brush.h
    wx/cocoa/button.h
    wx/cocoa/checkbox.h
    wx/cocoa/checklst.h
    wx/cocoa/choice.h
    wx/cocoa/clipbrd.h
    wx/cocoa/colordlg.h
    wx/cocoa/colour.h
    wx/cocoa/combobox.h
    wx/cocoa/control.h
    wx/cocoa/cursor.h
    wx/cocoa/dataform.h
    wx/cocoa/dataobj.h
    wx/cocoa/dataobj2.h
    wx/cocoa/dc.h
    wx/cocoa/dcclient.h
    wx/cocoa/dcmemory.h
    wx/cocoa/dcscreen.h
    wx/cocoa/dialog.h
    wx/cocoa/dirdlg.h
    wx/cocoa/drawer.h
    wx/cocoa/evtloop.h
    wx/cocoa/filedlg.h
    wx/cocoa/font.h
    wx/cocoa/fontdlg.h
    wx/cocoa/frame.h
    wx/cocoa/gauge.h
    wx/cocoa/icon.h
    wx/cocoa/listbox.h
    wx/cocoa/log.h
    wx/cocoa/mbarman.h
    wx/cocoa/mdi.h
    wx/cocoa/menu.h
    wx/cocoa/menuitem.h
    wx/cocoa/msgdlg.h
    wx/cocoa/notebook.h
    wx/cocoa/pen.h
    wx/cocoa/radiobox.h
    wx/cocoa/radiobut.h
    wx/cocoa/region.h
    wx/cocoa/scrolbar.h
    wx/cocoa/slider.h
    wx/cocoa/sound.h
    wx/cocoa/spinbutt.h
    wx/cocoa/statbmp.h
    wx/cocoa/statbox.h
    wx/cocoa/statline.h
    wx/cocoa/stattext.h
    wx/cocoa/textctrl.h
    wx/cocoa/toolbar.h
    wx/cocoa/tooltip.h
    wx/cocoa/toplevel.h
    wx/cocoa/window.h
    ## Semi-private headers
    wx/cocoa/objc/objc_uniquifying.h
    ## Generic implementations used by wxCocoa:
    wx/generic/caret.h
    wx/generic/clrpickerg.h
    wx/generic/collpaneg.h
    wx/generic/fdrepdlg.h
    wx/generic/fontdlgg.h
    wx/generic/fontpickerg.h
    wx/generic/imaglist.h
    wx/generic/listctrl.h
    wx/generic/paletteg.h
    wx/generic/region.h
    wx/generic/statusbr.h
)



## ======================================================================
##                              wxUniversal
## ======================================================================


set(UNIV_THEMES_SRC
    src/univ/themes/gtk.cpp
    src/univ/themes/metal.cpp
    src/univ/themes/mono.cpp
    src/univ/themes/win32.cpp
)

set(UNIV_SRC
    src/generic/accel.cpp
    src/generic/clrpickerg.cpp
    src/generic/collpaneg.cpp
    src/generic/colrdlgg.cpp
    src/generic/dirdlgg.cpp
    src/generic/fdrepdlg.cpp
    src/generic/filedlgg.cpp
    src/generic/filepickerg.cpp
    src/generic/fontdlgg.cpp
    src/generic/fontpickerg.cpp
    src/generic/listctrl.cpp
    src/generic/mdig.cpp
    src/generic/msgdlgg.cpp
    src/generic/prntdlgg.cpp
    src/univ/bmpbuttn.cpp
    src/univ/button.cpp
    src/univ/checkbox.cpp
    src/univ/checklst.cpp
    src/univ/choice.cpp
    src/univ/combobox.cpp
    src/univ/control.cpp
    src/univ/ctrlrend.cpp
    src/univ/dialog.cpp
    src/univ/framuniv.cpp
    src/univ/gauge.cpp
    src/univ/inpcons.cpp
    src/univ/inphand.cpp
    src/univ/listbox.cpp
    src/univ/menu.cpp
    src/univ/notebook.cpp
    src/univ/radiobox.cpp
    src/univ/radiobut.cpp
    src/univ/scrarrow.cpp
    src/univ/scrolbar.cpp
    src/univ/scrthumb.cpp
    src/univ/settingsuniv.cpp
    src/univ/slider.cpp
    src/univ/spinbutt.cpp
    src/univ/statbmp.cpp
    src/univ/statbox.cpp
    src/univ/statline.cpp
    src/univ/stattext.cpp
    src/univ/statusbr.cpp
    src/univ/stdrend.cpp
    src/univ/textctrl.cpp
    src/univ/tglbtn.cpp
    src/univ/theme.cpp
    src/univ/toolbar.cpp
    src/univ/topluniv.cpp
    src/univ/winuniv.cpp
)
set(UNIV_HDR
    wx/generic/accel.h
    wx/generic/animate.h
    wx/generic/clrpickerg.h
    wx/generic/collpaneg.h
    wx/generic/ctrlsub.h
    wx/generic/dirdlgg.h
    wx/generic/fdrepdlg.h
    wx/generic/filedlgg.h
    wx/generic/filepickerg.h
    wx/generic/fontdlgg.h
    wx/generic/fontpickerg.h
    wx/generic/listctrl.h
    wx/generic/mdig.h
    wx/generic/statusbr.h
    wx/univ/app.h
    wx/univ/bmpbuttn.h
    wx/univ/button.h
    wx/univ/checkbox.h
    wx/univ/checklst.h
    wx/univ/chkconf.h
    wx/univ/choice.h
    wx/univ/colschem.h
    wx/univ/combobox.h
    wx/univ/control.h
    wx/univ/dialog.h
    wx/univ/frame.h
    wx/univ/gauge.h
    wx/univ/inpcons.h
    wx/univ/inphand.h
    wx/univ/listbox.h
    wx/univ/menu.h
    wx/univ/menuitem.h
    wx/univ/notebook.h
    wx/univ/radiobox.h
    wx/univ/radiobut.h
    wx/univ/renderer.h
    wx/univ/scrarrow.h
    wx/univ/scrolbar.h
    wx/univ/scrthumb.h
    wx/univ/scrtimer.h
    wx/univ/slider.h
    wx/univ/spinbutt.h
    wx/univ/statbmp.h
    wx/univ/statbox.h
    wx/univ/statline.h
    wx/univ/stattext.h
    wx/univ/statusbr.h
    wx/univ/stdrend.h
    wx/univ/textctrl.h
    wx/univ/theme.h
    wx/univ/tglbtn.h
    wx/univ/toolbar.h
    wx/univ/toplevel.h
    wx/univ/window.h
)

## ======================================================================
##                               wxAdvanced
## ======================================================================

set(ADVANCED_CMN_SRC
    src/common/animatecmn.cpp
    src/common/bmpcboxcmn.cpp
    src/common/calctrlcmn.cpp
    src/common/datavcmn.cpp
    src/common/hyperlnkcmn.cpp
    src/generic/aboutdlgg.cpp
    src/generic/bmpcboxg.cpp
    src/generic/calctrlg.cpp
    src/generic/datavgen.cpp
    src/generic/datectlg.cpp
    src/generic/editlbox.cpp
    src/generic/grid.cpp
    src/generic/gridctrl.cpp
    src/generic/grideditors.cpp
    src/generic/gridsel.cpp
    src/generic/helpext.cpp
    src/generic/hyperlinkg.cpp
    src/generic/laywin.cpp
    src/generic/notifmsgg.cpp
    src/generic/odcombo.cpp
    src/generic/propdlg.cpp
    src/generic/sashwin.cpp
    src/generic/splash.cpp
    src/generic/tipdlg.cpp
    src/generic/wizard.cpp
)

set(ADVANCED_CMN_HDR
    wx/aboutdlg.h
    wx/animate.h
    wx/bmpcbox.h
    wx/calctrl.h
    wx/dataview.h
    wx/datectrl.h
    wx/dateevt.h
    wx/dcbuffer.h
    wx/editlbox.h
    wx/generic/aboutdlgg.h
    wx/generic/bmpcbox.h
    wx/generic/calctrlg.h
    wx/generic/datectrl.h
    wx/generic/dataview.h
    wx/generic/grid.h
    wx/generic/gridctrl.h
    wx/generic/grideditors.h
    wx/generic/gridsel.h
    wx/generic/helpext.h
    wx/generic/hyperlink.h
    wx/generic/laywin.h
    wx/generic/notifmsg.h
    wx/generic/propdlg.h
    wx/generic/sashwin.h
    wx/generic/splash.h
    wx/generic/wizard.h
    wx/grid.h
    wx/hyperlink.h
    wx/joystick.h
    wx/laywin.h
    wx/notifmsg.h
    wx/odcombo.h
    wx/propdlg.h
    wx/sashwin.h
    wx/sound.h
    wx/splash.h
    wx/taskbar.h
    wx/tipdlg.h
    wx/wizard.h
)

set(ADVANCED_MSW_SRC
    src/common/taskbarcmn.cpp
    src/msw/aboutdlg.cpp
    src/msw/notifmsg.cpp
    src/msw/sound.cpp
    src/msw/taskbar.cpp
)
set(ADVANCED_MSW_HDR
    wx/msw/notifmsg.h
    wx/msw/sound.h
    wx/msw/taskbar.h
)

## not used with wxUniv
set(ADVANCED_MSW_NATIVE_SRC
    src/generic/animateg.cpp
    src/msw/bmpcbox.cpp
    src/msw/calctrl.cpp
    src/msw/datecontrols.cpp
    src/msw/datectrl.cpp
)
set(ADVANCED_MSW_NATIVE_HDR
    wx/generic/animate.h
    wx/msw/bmpcbox.h
    wx/msw/calctrl.h
    wx/msw/datectrl.h
)

## not built on WindowsCE:
set(ADVANCED_MSW_DESKTOP_SRC
    src/msw/joystick.cpp
)
set(ADVANCED_MSW_DESKTOP_HDR
    wx/msw/joystick.h
)

set(ADVANCED_OSX_CARBON_SRC
    src/common/taskbarcmn.cpp
    src/generic/animateg.cpp
    src/osx/carbon/aboutdlg.cpp
    src/osx/carbon/drawer.cpp
    src/osx/carbon/dataview.cpp
    src/osx/carbon/databrow.cpp
    src/osx/carbon/sound.cpp
    src/osx/carbon/taskbar.cpp
    src/osx/core/hidjoystick.cpp
)

set(ADVANCED_OSX_CARBON_HDR
    wx/generic/animate.h
    wx/osx/dataview.h
    wx/osx/joystick.h
    wx/osx/sound.h
    wx/osx/taskbarosx.h
    wx/osx/carbon/drawer.h
    wx/osx/carbon/dataview.h
    wx/osx/carbon/databrow.h
    wx/osx/core/joystick.h
)

set(ADVANCED_OSX_COCOA_SRC
    src/common/taskbarcmn.cpp
    src/generic/animateg.cpp
    src/osx/carbon/sound.cpp
    src/osx/cocoa/aboutdlg.mm
    src/osx/cocoa/taskbar.mm
    src/osx/core/hidjoystick.cpp
)

set(ADVANCED_OSX_COCOA_HDR
    wx/generic/animate.h
    wx/osx/dataview.h
    wx/osx/joystick.h
    wx/osx/sound.h
    wx/osx/taskbarosx.h
    wx/osx/core/joystick.h
)

set(ADVANCED_COCOA_SRC
    src/cocoa/taskbar.mm
    src/common/taskbarcmn.cpp
    src/generic/animateg.cpp
    src/osx/core/hidjoystick.cpp
)

set(ADVANCED_COCOA_HDR
    wx/cocoa/taskbar.h
    wx/generic/animate.h
    wx/osx/core/joystick.h
)

set(ADVANCED_OS2_SRC
    src/generic/animateg.cpp
    src/os2/joystick.cpp
    src/os2/sound.cpp
)
set(ADVANCED_OS2_HDR
    wx/generic/animate.h
    wx/os2/joystick.h
    wx/os2/sound.h
)

set(ADVANCED_UNIX_SRC
    src/common/taskbarcmn.cpp
    src/unix/joystick.cpp
    src/unix/sound.cpp
    src/unix/taskbarx11.cpp
)
set(ADVANCED_UNIX_HDR
    wx/unix/joystick.h
    wx/unix/sound.h
    wx/unix/taskbarx11.h
)

set(ADVANCED_MOTIF_SRC
    src/generic/animateg.cpp
)

set(ADVANCED_MOTIF_HDR
    wx/generic/animate.h
)

set(ADVANCED_GTK_SRC
    src/gtk/taskbar.cpp
    src/gtk/eggtrayicon.c
    src/gtk/hildon/notifmsg.cpp
)
set(ADVANCED_GTK_HDR
    wx/gtk/hildon/notifmsg.h
    wx/gtk/taskbar.h
)

set(ADVANCED_GTK_NATIVE_SRC
    src/gtk/aboutdlg.cpp
    src/gtk/animate.cpp
    src/gtk/bmpcbox.cpp
    src/gtk/calctrl.cpp
    src/gtk/dataview.cpp
    src/gtk/hyperlink.cpp
)
set(ADVANCED_GTK_NATIVE_HDR
    wx/gtk/animate.h
    wx/gtk/bmpcbox.h
    wx/gtk/calctrl.h
    wx/gtk/dataview.h
    wx/gtk/hyperlink.h
)

set(ADVANCED_GTK1_SRC
    src/generic/animateg.cpp
    src/gtk1/eggtrayicon.c
    src/gtk1/taskbar.cpp
)
set(ADVANCED_GTK1_HDR
    wx/generic/animate.h
)

set(ADVANCED_UNIV_SRC
    src/generic/animateg.cpp
)
set(ADVANCED_UNIV_HDR
    wx/generic/animate.h
)

## ======================================================================
##                               wxMedia
## ======================================================================

set(MEDIA_CMN_SRC
    src/common/mediactrlcmn.cpp
)

set(MEDIA_CMN_HDR
    wx/mediactrl.h
)

set(MEDIA_MSW_SRC
    src/msw/mediactrl_am.cpp
    src/msw/mediactrl_wmp10.cpp
    src/msw/mediactrl_qt.cpp
)
set(MEDIA_MSW_HDR
)
## not built on WindowsCE:
set(MEDIA_MSW_DESKTOP_SRC
)
set(MEDIA_MSW_DESKTOP_HDR
)

set(MEDIA_OSX_CARBON_SRC
    src/osx/carbon/mediactrl.cpp
)
set(MEDIA_OSX_CARBON_HDR
)

set(MEDIA_OSX_COCOA_SRC
    ## src/osx/cocoa/mediactrl.mm
)
set(MEDIA_OSX_COCOA_HDR
)

set(MEDIA_COCOA_SRC
    src/cocoa/mediactrl.mm
)
set(MEDIA_COCOA_HDR
)

set(MEDIA_OS2_SRC
)
set(MEDIA_OS2_HDR
)

set(MEDIA_UNIX_SRC
    src/unix/mediactrl.cpp
)
set(MEDIA_UNIX_HDR
)

set(MEDIA_GTK_SRC
)

set(MEDIA_GTK1_SRC
)

## ======================================================================
##                               wxHTML
## ======================================================================

if (WIN32)
    set(HTML_SRC_PLATFORM src/msw/helpbest.cpp)
 else ()
    set(HTML_SRC_PLATFORM  src/html/chm.cpp)
 endif()


set(HTML_SRC
    ${HTML_SRC_PLATFORM}
    src/html/helpctrl.cpp
    src/html/helpdata.cpp
    src/html/helpdlg.cpp
    src/html/helpfrm.cpp
    src/html/helpwnd.cpp
    src/html/htmlcell.cpp
    src/html/htmlfilt.cpp
    src/html/htmlpars.cpp
    src/html/htmltag.cpp
    src/html/htmlwin.cpp
    src/html/htmprint.cpp
    src/html/m_dflist.cpp
    src/html/m_fonts.cpp
    src/html/m_hline.cpp
    src/html/m_image.cpp
    src/html/m_layout.cpp
    src/html/m_links.cpp
    src/html/m_list.cpp
    src/html/m_pre.cpp
    src/html/m_style.cpp
    src/html/m_tables.cpp
    src/html/winpars.cpp
    ## wxHTML users:
    src/generic/htmllbox.cpp
)

set(MSW_HTML_HDR
    wx/msw/helpbest.h
    )

set(HTML_HDR
    wx/html/helpctrl.h
    wx/html/helpdata.h
    wx/html/helpdlg.h
    wx/html/helpfrm.h
    wx/html/helpwnd.h
    wx/html/htmlcell.h
    wx/html/htmldefs.h
    wx/html/htmlfilt.h
    wx/html/htmlpars.h
    wx/html/htmlproc.h
    wx/html/htmltag.h
    wx/html/htmlwin.h
    wx/html/htmprint.h
    wx/html/m_templ.h
    wx/html/winpars.h
    wx/wxhtml.h
    ## wxHTML users:
    wx/htmllbox.h
    ${MSW_HTML_HDR}
)



## ======================================================================
##                                wxXRC
## ======================================================================

set(XRC_SRC
    src/xrc/xh_animatctrl.cpp
    src/xrc/xh_bmp.cpp
    src/xrc/xh_bmpcbox.cpp
    src/xrc/xh_bmpbt.cpp
    src/xrc/xh_bttn.cpp
    src/xrc/xh_cald.cpp
    src/xrc/xh_chckb.cpp
    src/xrc/xh_chckl.cpp
    src/xrc/xh_choic.cpp
    src/xrc/xh_choicbk.cpp
    src/xrc/xh_clrpicker.cpp
    src/xrc/xh_collpane.cpp
    src/xrc/xh_combo.cpp
    src/xrc/xh_comboctrl.cpp
    src/xrc/xh_datectrl.cpp
    src/xrc/xh_dirpicker.cpp
    src/xrc/xh_dlg.cpp
    src/xrc/xh_frame.cpp
    src/xrc/xh_mdi.cpp
    src/xrc/xh_filepicker.cpp
    src/xrc/xh_fontpicker.cpp
    src/xrc/xh_gauge.cpp
    src/xrc/xh_gdctl.cpp
    src/xrc/xh_grid.cpp
    src/xrc/xh_html.cpp
    src/xrc/xh_hyperlink.cpp
    src/xrc/xh_listb.cpp
    src/xrc/xh_listbk.cpp
    src/xrc/xh_listc.cpp
    src/xrc/xh_menu.cpp
    src/xrc/xh_notbk.cpp
    src/xrc/xh_odcombo.cpp
    src/xrc/xh_panel.cpp
    src/xrc/xh_propdlg.cpp
    src/xrc/xh_radbt.cpp
    src/xrc/xh_radbx.cpp
    ## disabled until linking problems are fixed
    ## src/xrc/xh_richtext.cpp
    src/xrc/xh_scrol.cpp
    src/xrc/xh_scwin.cpp
    src/xrc/xh_htmllbox.cpp
    src/xrc/xh_sizer.cpp
    src/xrc/xh_slidr.cpp
    src/xrc/xh_spin.cpp
    src/xrc/xh_split.cpp
    src/xrc/xh_srchctrl.cpp
    src/xrc/xh_statbar.cpp
    src/xrc/xh_stbmp.cpp
    src/xrc/xh_stbox.cpp
    src/xrc/xh_stlin.cpp
    src/xrc/xh_sttxt.cpp
    src/xrc/xh_text.cpp
    src/xrc/xh_tglbtn.cpp
    src/xrc/xh_toolb.cpp
    src/xrc/xh_tree.cpp
    src/xrc/xh_treebk.cpp
    src/xrc/xh_unkwn.cpp
    src/xrc/xh_wizrd.cpp
    src/xrc/xmlres.cpp
    src/xrc/xmladv.cpp
    src/xrc/xmlrsall.cpp
)
set(XRC_HDR
    wx/xrc/xh_all.h
    wx/xrc/xh_animatctrl.h
    wx/xrc/xh_bmpbt.h
    wx/xrc/xh_bmp.h
    wx/xrc/xh_bmpcbox.h
    wx/xrc/xh_bttn.h
    wx/xrc/xh_cald.h
    wx/xrc/xh_chckb.h
    wx/xrc/xh_chckl.h
    wx/xrc/xh_choic.h
    wx/xrc/xh_choicbk.h
    wx/xrc/xh_clrpicker.h
    wx/xrc/xh_collpane.h
    wx/xrc/xh_combo.h
    wx/xrc/xh_comboctrl.h
    wx/xrc/xh_datectrl.h
    wx/xrc/xh_dirpicker.h
    wx/xrc/xh_dlg.h
    wx/xrc/xh_filepicker.h
    wx/xrc/xh_fontpicker.h
    wx/xrc/xh_frame.h
    wx/xrc/xh_mdi.h
    wx/xrc/xh_gauge.h
    wx/xrc/xh_gdctl.h
    wx/xrc/xh_grid.h
    wx/xrc/xh_html.h
    wx/xrc/xh_hyperlink.h
    wx/xrc/xh_listb.h
    wx/xrc/xh_listbk.h
    wx/xrc/xh_listc.h
    wx/xrc/xh_menu.h
    wx/xrc/xh_notbk.h
    wx/xrc/xh_odcombo.h
    wx/xrc/xh_panel.h
    wx/xrc/xh_propdlg.h
    wx/xrc/xh_radbt.h
    wx/xrc/xh_radbx.h
    ## wx/xrc/xh_richtext.h
    wx/xrc/xh_scrol.h
    wx/xrc/xh_scwin.h
    wx/xrc/xh_htmllbox.h
    wx/xrc/xh_sizer.h
    wx/xrc/xh_slidr.h
    wx/xrc/xh_spin.h
    wx/xrc/xh_split.h
    wx/xrc/xh_srchctrl.h
    wx/xrc/xh_statbar.h
    wx/xrc/xh_stbmp.h
    wx/xrc/xh_stbox.h
    wx/xrc/xh_stlin.h
    wx/xrc/xh_sttxt.h
    wx/xrc/xh_text.h
    wx/xrc/xh_tglbtn.h
    wx/xrc/xh_toolb.h
    wx/xrc/xh_tree.h
    wx/xrc/xh_treebk.h
    wx/xrc/xh_unkwn.h
    wx/xrc/xh_wizrd.h
    wx/xrc/xmlres.h
)




## ======================================================================
##                               XML classes
## ======================================================================

set(XML_SRC
    src/xml/xml.cpp
    src/common/xtixml.cpp ## FIXME - temporary solution
)
set(XML_HDR
    wx/xml/xml.h
    wx/xtixml.h ## FIXME - temporary solution
)


## ======================================================================
##                             OpenGL canvas
## ======================================================================

set(OPENGL_SRC_PLATFORM_CARBON
    src/osx/carbon/glcanvas.cpp
    src/osx/glcanvas_osx.cpp
)

set(OPENGL_SRC_PLATFORM_COCOA
    src/osx/cocoa/glcanvas.mm
    src/osx/glcanvas_osx.cpp
    src/cocoa/glcanvas.mm
)

set(OPENGL_SRC_PLATFORM_GTK
    src/gtk/glcanvas.cpp
    src/unix/glx11.cpp
)

set(OPENGL_SRC_PLATFORM_MSW
    src/msw/glcanvas.cpp
)

if (WIN32)
    set(OPENGL_SRC_PLATFORM ${OPENGL_SRC_PLATFORM_MSW})
endif()

if (APPLE)
    set(OPENGL_SRC_PLATFORM ${OPENGL_SRC_PLATFORM_CARBON})
elseif (UNIX)
    set(OPENGL_SRC_PLATFORM ${OPENGL_SRC_PLATFORM_GTK})
endif()

set(OPENGL_SRC
    src/common/glcmn.cpp
    ${OPENGL_SRC_PLATFORM}
)

set(OPENGL_HDR_PLATFORM )

set(OPENGL_HDR
    wx/glcanvas.h
    ${OPENGL_HDR_PLATFORM}
)



## ======================================================================
##                                  wxAUI
## ======================================================================

set(AUI_SRC
    src/aui/framemanager.cpp
    src/aui/dockart.cpp
    src/aui/floatpane.cpp
    src/aui/auibook.cpp
    src/aui/auibar.cpp
    src/aui/tabmdi.cpp
)
set(AUI_HDR
    wx/aui/framemanager.h
    wx/aui/dockart.h
    wx/aui/floatpane.h
    wx/aui/auibook.h
    wx/aui/auibar.h
    wx/aui/tabmdi.h
    wx/aui/aui.h
)

## ======================================================================
##                                  wxPropertyGrid
## ======================================================================

set(PROPGRID_SRC
    src/propgrid/advprops.cpp
    src/propgrid/editors.cpp
    src/propgrid/manager.cpp
    src/propgrid/property.cpp
    src/propgrid/propgrid.cpp
    src/propgrid/propgridiface.cpp
    src/propgrid/propgridpagestate.cpp
    src/propgrid/props.cpp
)
set(PROPGRID_HDR
    wx/propgrid/advprops.h
    wx/propgrid/editors.h
    wx/propgrid/manager.h
    wx/propgrid/property.h
    wx/propgrid/propgrid.h
    wx/propgrid/propgriddefs.h
    wx/propgrid/propgridiface.h
    wx/propgrid/propgridpagestate.h
    wx/propgrid/props.h
)

## ======================================================================
##                                  wxRichTextCtrl
## ======================================================================

set(RICHTEXT_SRC
    src/richtext/richtextctrl.cpp
    src/richtext/richtextbuffer.cpp
    src/richtext/richtextstyles.cpp
    src/richtext/richtextxml.cpp
    src/richtext/richtexthtml.cpp
    src/richtext/richtextformatdlg.cpp
    src/richtext/richtextsymboldlg.cpp
    src/richtext/richtextstyledlg.cpp
    src/richtext/richtextprint.cpp
)
set(RICHTEXT_HDR
    wx/richtext/richtextctrl.h
    wx/richtext/richtextbuffer.h
    wx/richtext/richtexthtml.h
    wx/richtext/richtextstyles.h
    wx/richtext/richtextxml.h
    wx/richtext/richtextformatdlg.h
    wx/richtext/richtextsymboldlg.h
    wx/richtext/richtextstyledlg.h
    wx/richtext/richtextprint.h
)


## ======================================================================
##                                wxSTC
## ======================================================================

set(STC_SRC
    src/stc/stc.cpp
    src/stc/PlatWX.cpp
    src/stc/ScintillaWX.cpp
)

set(STC_HDR
    wx/stc/stc.h
)
