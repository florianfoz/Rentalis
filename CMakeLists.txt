cmake_minimum_required(VERSION 3.16)

project(Rentalis VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
    ./src/damage.cpp
    ./src/damage.h
    ./src/database.cpp
    ./src/database.h
    ./src/database_manager.cpp
    ./src/database_manager.h
    ./src/db_settings.cpp
    ./src/db_settings.h
    ./src/delegates.h
    ./src/image_buffer.cpp
    ./src/image_buffer.h
    ./src/invoice.cpp
    ./src/invoice.h
    ./src/landlord.cpp
    ./src/landlord.h
    ./src/lease_agreement.cpp
    ./src/lease_agreement.h
    ./src/main.cpp
    ./src/mainmenu.cpp
    ./src/mainmenu.h
    ./src/maintenance.cpp
    ./src/maintenance.h
    ./src/pch.h
    ./src/property.cpp
    ./src/property_feature.cpp
    ./src/property_feature.h
    ./src/property.h
    ./src/property_room.cpp
    ./src/property_room.h
    ./src/receipt.cpp
    ./src/receipt.h
    ./src/record_file.cpp
    ./src/record_file.h
    ./src/record_files.cpp
    ./src/record_files.h
    ./src/rent.cpp
    ./src/rent.h
    ./src/settings.cpp
    ./src/settings.h
    ./src/tenant.cpp
    ./src/tenant.h
    ./src/widget/w_ask_ai.cpp
    ./src/widget/w_ask_ai.h
    ./src/widget/w_comment_dialog.cpp
    ./src/widget/w_comment_dialog.h
    ./src/widget/w_damage.cpp
    ./src/widget/w_damage_creator.cpp
    ./src/widget/w_damage_creator.h
    ./src/widget/w_damage.h
    ./src/widget/w_damage_manager.cpp
    ./src/widget/w_damage_manager.h
    ./src/widget/w_dashboard_check_property.cpp
    ./src/widget/w_dashboard_check_property.h
    ./src/widget/w_dashboard_check_rents_distribution.cpp
    ./src/widget/w_dashboard_check_rents_distribution.h
    ./src/widget/w_dashboard.cpp
    ./src/widget/w_dashboard.h
    ./src/widget/w_database_choice.cpp
    ./src/widget/w_database_choice.h
    ./src/widget/w_database_creator.cpp
    ./src/widget/w_database_creator.h
    ./src/widget/w_database_current_info.cpp
    ./src/widget/w_database_current_info.h
    ./src/widget/w_database_importation.cpp
    ./src/widget/w_database_importation.h
    ./src/widget/w_files_view.cpp
    ./src/widget/w_files_view.h
    ./src/widget/w_flowlayout.cpp
    ./src/widget/w_flowlayout.h
    ./src/widget/w_invoice.cpp
    ./src/widget/w_invoice_creator.cpp
    ./src/widget/w_invoice_creator.h
    ./src/widget/w_invoice_form.cpp
    ./src/widget/w_invoice_form.h
    ./src/widget/w_invoice.h
    ./src/widget/w_invoice_manager.cpp
    ./src/widget/w_invoice_manager.h
    ./src/widget/w_landlord.cpp
    ./src/widget/w_landlord_creator.cpp
    ./src/widget/w_landlord_creator.h
    ./src/widget/w_landlord.h
    ./src/widget/w_landlord_manager.cpp
    ./src/widget/w_landlord_manager.h
    ./src/widget/w_maintenance.cpp
    ./src/widget/w_maintenance_creator.cpp
    ./src/widget/w_maintenance_creator.h
    ./src/widget/w_maintenance.h
    ./src/widget/w_maintenance_manager.cpp
    ./src/widget/w_maintenance_manager.h
    ./src/widget/w_property.cpp
    ./src/widget/w_property_creator.cpp
    ./src/widget/w_property_creator.h
    ./src/widget/w_property.h
    ./src/widget/w_property_manager.cpp
    ./src/widget/w_property_manager.h
    ./src/widget/w_property_room_creator.cpp
    ./src/widget/w_property_room_creator.h
    ./src/widget/w_receipt.cpp
    ./src/widget/w_receipt_creator.cpp
    ./src/widget/w_receipt_creator.h
    ./src/widget/w_receipt_form.cpp
    ./src/widget/w_receipt_form.h
    ./src/widget/w_receipt.h
    ./src/widget/w_receipt_manager.cpp
    ./src/widget/w_receipt_manager.h
    ./src/widget/w_rent.cpp
    ./src/widget/w_rent_histogram.cpp
    ./src/widget/w_rent_histogram.h
    ./src/widget/w_rent.h
    ./src/widget/w_rent_manager.cpp
    ./src/widget/w_rent_manager.h
    ./src/widget/w_rent_tool_declaration.cpp
    ./src/widget/w_rent_tool_declaration.h
    ./src/widget/w_rent_tool_revision.cpp
    ./src/widget/w_rent_tool_revision.h
    ./src/widget/w_rent_view.cpp
    ./src/widget/w_rent_view.h
    ./src/widget/w_save.cpp
    ./src/widget/w_save_creator.cpp
    ./src/widget/w_save_creator.h
    ./src/widget/w_save.h
    ./src/widget/w_save_manager.cpp
    ./src/widget/w_save_manager.h
    ./src/widget/w_settings.cpp
    ./src/widget/w_settings.h
    ./src/widget/w_tenant.cpp
    ./src/widget/w_tenant_creator.cpp
    ./src/widget/w_tenant_creator.h
    ./src/widget/w_tenant.h
    ./src/widget/w_tenant_manager.cpp
    ./src/widget/w_tenant_manager.h
    ./src/widget/w_tile_view.cpp
    ./src/widget/w_tile_view.h
    ./src/widget/w_welcome.cpp
    ./src/widget/w_welcome.h


)

# command getter
# find ./ui -type f -name "*.ui" | sort
set(UI

    ./ui/mainmenu.ui
    ./ui/w_ask_ai.ui
    ./ui/w_comment_dialog.ui
    ./ui/w_damage_creator.ui
    ./ui/w_damage_manager.ui
    ./ui/w_damage.ui
    ./ui/w_dashboard_check_property.ui
    ./ui/w_dashboard_check_rents_distribution.ui
    ./ui/w_dashboard.ui
    ./ui/w_database_choice.ui
    ./ui/w_database_creator.ui
    ./ui/w_database_current_info.ui
    ./ui/w_database_importation.ui
    ./ui/w_files_view.ui
    ./ui/w_invoice_creator.ui
    ./ui/w_invoice_form.ui
    ./ui/w_invoice_manager.ui
    ./ui/w_invoice.ui
    ./ui/w_landlord_creator.ui
    ./ui/w_landlord_manager.ui
    ./ui/w_landlord.ui
    ./ui/w_maintenance_creator.ui
    ./ui/w_maintenance_manager.ui
    ./ui/w_maintenance.ui
    ./ui/w_property_creator.ui
    ./ui/w_property_manager.ui
    ./ui/w_property_room_creator.ui
    ./ui/w_property.ui
    ./ui/w_receipt_creator.ui
    ./ui/w_receipt_form.ui
    ./ui/w_receipt_manager.ui
    ./ui/w_receipt.ui
    ./ui/w_rent_histogram.ui
    ./ui/w_rent_manager.ui
    ./ui/w_rent_tool_declaration.ui
    ./ui/w_rent_tool_revision.ui
    ./ui/w_rent.ui
    ./ui/w_rent_view.ui
    ./ui/w_save_creator.ui
    ./ui/w_save_manager.ui
    ./ui/w_save.ui
    ./ui/w_settings.ui
    ./ui/w_tenant_creator.ui
    ./ui/w_tenant_manager.ui
    ./ui/w_tenant.ui
    ./ui/w_tile_view.ui
    ./ui/w_welcome.ui

)


set(CMAKE_AUTOUIC_SEARCH_PATHS
    ${CMAKE_CURRENT_SOURCE_DIR}/ui
)


qt_add_executable(Rentalis
    ${SRC}
    ${UI}
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
