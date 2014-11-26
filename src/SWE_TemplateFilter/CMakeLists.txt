# add the include directory of the filter
include_directories(../include)
# declare our filter
adtf_add_filter(SWE_TemplateFilter
    stdafx.h
    stdafx.cpp

    SWE_TemplateFilter.h
    SWE_TemplateFilter.cpp
)

adtf_install_plugin(SWE_TemplateFilter ${AADC_INSTALL_DIR})
