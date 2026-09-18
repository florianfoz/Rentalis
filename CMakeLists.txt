cmake_minimum_required(VERSION 3.21)

project(Rentalis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt6 REQUIRED COMPONENTS Quick Sql Pdf Charts PrintSupport)

qt_standard_project_setup(REQUIRES 6.5)

# Content of downloaded design from Figma to Qt or similar
if (EXISTS "${CMAKE_CURRENT_LIST_DIR}/importedcontent/CMakeLists.txt")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/importedcontent)
endif()


# command getter
# find ./src/ -type f \( -name "*.cpp" -o -name "*.h" \) | sort
set(SRC

./src/base.cpp
./src/base.h
./src/csshighlighter.cpp
./src/csshighlighter.h
./src/database/database.cpp
./src/database/database.h
./src/database/manager.cpp
./src/database/manager.h
./src/database/manifest.cpp
./src/database/manifest.h
./src/database/recorder.cpp
./src/database/recorder.h
./src/delegates.h
./src/entity/attachment.cpp
./src/entity/attachment.h
./src/entity/damage.h
./src/entity/invoice.h
./src/entity/landlord.h
./src/entity/lease_agreement.h
./src/entity/maintenance.h
./src/entity/property.cpp
./src/entity/property_feature.h
./src/entity/property.h
./src/entity/property_room.h
./src/entity/receipt.cpp
./src/entity/receipt.h
./src/entity/rent.cpp
./src/entity/rent.h
./src/entity/tenant.cpp
./src/entity/tenant.h
./src/enum_lite.h
./src/forward.h
./src/main.cpp
./src/pch.h
./src/preferences.cpp
./src/preferences.h
./src/record/field/field.cpp
./src/record/field/field.h
./src/record/record.cpp
./src/record/record.h
./src/record/relation.h
./src/savemanager.cpp
./src/savemanager.h
./src/widget/database/w_database.cpp
./src/widget/database/w_database_creator.cpp
./src/widget/database/w_database_creator.h
./src/widget/database/w_database_current_info.cpp
./src/widget/database/w_database_current_info.h
./src/widget/database/w_database.h
./src/widget/database/w_database_importation.cpp
./src/widget/database/w_database_importation.h
./src/widget/database/w_database_manager.cpp
./src/widget/database/w_database_manager.h
./src/widget/entity/w_comment_dialog.cpp
./src/widget/entity/w_comment_dialog.h
./src/widget/entity/w_damage.cpp
./src/widget/entity/w_damage_creator.cpp
./src/widget/entity/w_damage_creator.h
./src/widget/entity/w_damage.h
./src/widget/entity/w_damage_manager.cpp
./src/widget/entity/w_damage_manager.h
./src/widget/entity/w_invoice.cpp
./src/widget/entity/w_invoice_creator.cpp
./src/widget/entity/w_invoice_creator.h
./src/widget/entity/w_invoice_form.cpp
./src/widget/entity/w_invoice_form.h
./src/widget/entity/w_invoice.h
./src/widget/entity/w_invoice_manager.cpp
./src/widget/entity/w_invoice_manager.h
./src/widget/entity/w_landlord.cpp
./src/widget/entity/w_landlord_creator.cpp
./src/widget/entity/w_landlord_creator.h
./src/widget/entity/w_landlord.h
./src/widget/entity/w_landlord_manager.cpp
./src/widget/entity/w_landlord_manager.h
./src/widget/entity/w_maintenance.cpp
./src/widget/entity/w_maintenance_creator.cpp
./src/widget/entity/w_maintenance_creator.h
./src/widget/entity/w_maintenance.h
./src/widget/entity/w_maintenance_manager.cpp
./src/widget/entity/w_maintenance_manager.h
./src/widget/entity/w_property.cpp
./src/widget/entity/w_property_creator.cpp
./src/widget/entity/w_property_creator.h
./src/widget/entity/w_property.h
./src/widget/entity/w_property_manager.cpp
./src/widget/entity/w_property_manager.h
./src/widget/entity/w_property_room_creator.cpp
./src/widget/entity/w_property_room_creator.h
./src/widget/entity/w_receipt.cpp
./src/widget/entity/w_receipt_creator.cpp
./src/widget/entity/w_receipt_creator.h
./src/widget/entity/w_receipt_form.cpp
./src/widget/entity/w_receipt_form.h
./src/widget/entity/w_receipt.h
./src/widget/entity/w_receipt_manager.cpp
./src/widget/entity/w_receipt_manager.h
./src/widget/entity/w_rent.cpp
./src/widget/entity/w_rent.h
./src/widget/entity/w_rent_histogram.cpp
./src/widget/entity/w_rent_histogram.h
./src/widget/entity/w_rent_manager.cpp
./src/widget/entity/w_rent_manager.h
./src/widget/entity/w_rent_tool_declaration.cpp
./src/widget/entity/w_rent_tool_declaration.h
./src/widget/entity/w_rent_tool_revision.cpp
./src/widget/entity/w_rent_tool_revision.h
./src/widget/entity/w_rent_view.cpp
./src/widget/entity/w_rent_view.h
./src/widget/entity/w_tenant.cpp
./src/widget/entity/w_tenant_creator.cpp
./src/widget/entity/w_tenant_creator.h
./src/widget/entity/w_tenant.h
./src/widget/entity/w_tenant_manager.cpp
./src/widget/entity/w_tenant_manager.h
./src/widget/w_ask_ai.cpp
./src/widget/w_ask_ai.h
./src/widget/w_dashboard_check_property.cpp
./src/widget/w_dashboard_check_property.h
./src/widget/w_dashboard_check_rents_distribution.cpp
./src/widget/w_dashboard_check_rents_distribution.h
./src/widget/w_dashboard.cpp
./src/widget/w_dashboard.h
./src/widget/w_files_view.cpp
./src/widget/w_files_view.h
./src/widget/w_first_welcome.cpp
./src/widget/w_first_welcome.h
./src/widget/w_flowlayout.cpp
./src/widget/w_flowlayout.h
./src/widget/w_help.cpp
./src/widget/w_help.h
./src/widget/w_mainmenu.cpp
./src/widget/w_mainmenu.h
./src/widget/w_preferences.cpp
./src/widget/w_preferences.h
./src/widget/w_tile_view.cpp
./src/widget/w_tile_view.h
./src/widget/w_welcome.cpp
./src/widget/w_welcome.h


)

# command getter
# find ./ui -type f -name "*.ui" | sort
set(UI
      
./ui/database/w_database_creator.ui
./ui/database/w_database_current_info.ui
./ui/database/w_database_importation.ui
./ui/database/w_database_manager.ui
./ui/database/w_database.ui
./ui/entity/w_comment_dialog.ui
./ui/entity/w_damage_creator.ui
./ui/entity/w_damage_manager.ui
./ui/entity/w_damage.ui
./ui/entity/w_invoice_creator.ui
./ui/entity/w_invoice_form.ui
./ui/entity/w_invoice_manager.ui
./ui/entity/w_invoice.ui
./ui/entity/w_landlord_creator.ui
./ui/entity/w_landlord_manager.ui
./ui/entity/w_landlord.ui
./ui/entity/w_maintenance_creator.ui
./ui/entity/w_maintenance_manager.ui
./ui/entity/w_maintenance.ui
./ui/entity/w_property_creator.ui
./ui/entity/w_property_manager.ui
./ui/entity/w_property_room_creator.ui
./ui/entity/w_property.ui
./ui/entity/w_receipt_creator.ui
./ui/entity/w_receipt_form.ui
./ui/entity/w_receipt_manager.ui
./ui/entity/w_receipt.ui
./ui/entity/w_rent_histogram.ui
./ui/entity/w_rent_manager.ui
./ui/entity/w_rent_tool_declaration.ui
./ui/entity/w_rent_tool_revision.ui
./ui/entity/w_rent.ui
./ui/entity/w_rent_view.ui
./ui/entity/w_tenant_creator.ui
./ui/entity/w_tenant_manager.ui
./ui/entity/w_tenant.ui
./ui/w_mainmenu.ui
./ui/w_ask_ai.ui
./ui/w_dashboard_check_property.ui
./ui/w_dashboard_check_rents_distribution.ui
./ui/w_dashboard.ui
./ui/w_files_view.ui
./ui/w_first_welcome.ui
./ui/w_help.ui
./ui/w_preferences.ui
./ui/w_tile_view.ui
./ui/w_welcome.ui


)


set(QRC
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/database.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/themes.qrc
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/translations.qrc
)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/database
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/entity
)


qt_add_executable(Rentalis
    ${SRC}
    ${UI}
    ${QRC}
)

file(GLOB_RECURSE DOCS_FILES CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/resources/docs/*"
)

qt_add_resources(Rentalis "docs" 
    PREFIX "/docs"
    BASE "${CMAKE_SOURCE_DIR}"
    FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE"
        "${CMAKE_CURRENT_SOURCE_DIR}/NOTICE"
        "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
        ${DOCS_FILES}
)

target_include_directories(Rentalis
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/src/widget
)

target_precompile_headers(Rentalis
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src/pch.h
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(Rentalis PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Rentalis
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(Rentalis
    PRIVATE Qt6::Quick
    Qt6::Sql
    Qt6::Pdf
    Qt6::Charts
    Qt6::PrintSupport
)

install(TARGETS Rentalis
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
