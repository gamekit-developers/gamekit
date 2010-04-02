if (UNIX)
    include(CheckTypeSize)
    message("WARNING: Type checks in ${wxWidgets_SOURCE_DIR}/wxCMake/setup.h.in may need to be added to match your system specs.")

    # sizes are the main focus
    # other checks may need to be added
    check_type_size(short            SIZEOF_SHORT)
    check_type_size(int              SIZEOF_INT)
    check_type_size(long             SIZEOF_LONG)
    check_type_size(wchar_t          SIZEOF_WCHAR_T)
    check_type_size(void*            SIZEOF_VOID_P)
    check_type_size("long long"      SIZEOF_LONG_LONG)
    check_type_size(size_t           SIZEOF_SIZE_T)
    check_type_size("unsigned int"   SIZEOF_UINT)


    if (${SIZEOF_SIZE_T} EQUAL ${SIZEOF_UINT})
        set(wxSIZE_T_IS_UINT 1)
    else()
        set(wxSIZE_T_IS_ULONG 1)
    endif()

    configure_file(${wxWidgets_SOURCE_DIR}/wxCMake/setup.h.in ${wxWidgets_SOURCE_DIR}/include/setup/wx/Unix/setup.h)


endif()
