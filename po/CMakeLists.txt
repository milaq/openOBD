project (po)

find_program (_intltool_update intltool-update)
mark_as_advanced (intltool_update)

find_program (_xgettext xgettext)
mark_as_advanced (_xgettext)

find_program (_msgmerge msgmerge)
mark_as_advanced (_msgmerge)

find_program(_msgfmt msgfmt)
mark_as_advanced (k3d_msgfmt)

set (openobd_catalogs ${ALL_LINGUAS})

set (openobd_catalog_targets)

add_custom_target (nls-update-sources
    ${_intltool_update} -m
    WORKING_DIRECTORY ${po_SOURCE_DIR}
)

add_custom_command (
    OUTPUT ${po_SOURCE_DIR}/openobd.pot
    COMMAND ${_intltool_update} --gettext-package openobd --pot
    DEPENDS POTFILES.in
    WORKING_DIRECTORY ${po_SOURCE_DIR}
)

foreach(catalog ${openobd_catalogs})
    add_custom_target (nls-update-${catalog}.po
	${_msgmerge} ${catalog} openobd -o ${catalog}.po --no-wrap
	DEPENDS ${po_SOURCE_DIR}/${catalog}.po
	WORKING_DIRECTORY ${po_SOURCE_DIR}
    )

    file (MAKE_DIRECTORY "${po_BINARY_DIR}/${catalog}/lc_messages")

    add_custom_command (
	OUTPUT ${po_BINARY_DIR}/${catalog}/lc_messages/openobd.mo
	COMMAND ${_msgfmt} -c -o ${po_BINARY_DIR}/${catalog}/lc_messages/openobd.mo ${po_SOURCE_DIR}/${catalog}.po
	DEPENDS ${po_SOURCE_DIR}/${catalog}.po
    )

    install (
	FILES ${po_BINARY_DIR}/${catalog}/lc_messages/openobd.mo
	DESTINATION ${OPENOBD_DESTDIR}${datadir}/locale/${catalog}/lc_messages
    )

    set (openobd_catalog_targets ${openobd_catalog_targets} ${po_BINARY_DIR}/${catalog}/lc_messages/openobd.mo)
endforeach(catalog)

add_custom_target(nls ALL
    DEPENDS ${po_SOURCE_DIR}/openobd.pot ${openobd_catalog_targets}
)

