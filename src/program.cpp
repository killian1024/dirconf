/* dirconf
 * Copyright (C) 2018 Killian Poulaud.
 *
 * This file is part of dirconf.
 *
 * dirconf is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * dirconf is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with speed. If not, see <http://www.gnu.org/licenses/>.
 */

//
// Created by Killian on 15/07/18.
//

#include <gio/gio.h>
#include <glib.h>

#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>

#include "program.hpp"


namespace dirconf {


program::program(
        std::filesystem::path src_pth,
        std::vector<std::string> conf_nmes,
        spdcontain::flags<file_managers> fle_managrs
)
        : src_pth_(std::move(src_pth))
        , conf_nmes_(std::move(conf_nmes))
        , fle_managrs_(std::move(fle_managrs))
{
}


int program::execute() const
{
    return execute_in_directory(src_pth_, std::filesystem::path()) ? 0 : -1;
}


bool program::execute_in_directory(
        const std::filesystem::path& dir_pth,
        const std::filesystem::path& last_conf_pth
) const
{
    std::filesystem::path conf_pth;
    bool found = false;
    bool sucss = true;
    bool recur = false;
    
    conf_pth = dir_pth;
    conf_pth /= ".";
    
    for (auto& x : conf_nmes_)
    {
        conf_pth.replace_filename(x);
        
        if (spdsys::is_regular_file(conf_pth.c_str()))
        {
            found = true;
            break;
        }
    }
    if (!found && !last_conf_pth.empty())
    {
        conf_pth = last_conf_pth;
        found = true;
    }
    if (found)
    {
        std::cout << spdios::set_light_blue_text
                  << "Applying configuration " << conf_pth.filename() << " in " << dir_pth;
        
        if (!apply_configuration(dir_pth, conf_pth, &recur))
        {
            sucss = false;
            std::cout << spdios::set_light_red_text << " [fail]" << spdios::newl;
        }
        else
        {
            std::cout << spdios::set_light_green_text << " [ok]" << spdios::newl;
        }
    }
    
    try
    {
        for (auto& x : std::filesystem::directory_iterator(dir_pth))
        {
            if (std::filesystem::is_directory(x) || std::filesystem::is_symlink(x))
            {
                if (!execute_in_directory(x.path(), recur ? conf_pth : std::filesystem::path()))
                {
                    sucss = false;
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error& fe)
    {
        std::cerr << spdios::set_light_red_text
                  << "Error executing in directory: " << dir_pth
                  << spdios::newl;
        
        return false;
    }
    
    return sucss;
}


bool program::apply_configuration(
        const std::filesystem::path& dir_pth,
        const std::filesystem::path& conf_pth,
        bool* recur
) const
{
    namespace pt = boost::property_tree;
    
    bool sucss = true;
    *recur = false;
    
    try
    {
        pt::ptree root;
        std::string icon_pth;
        std::string vie;
        std::size_t zoom_levl;
        bool compact_layt;
        const char* val;
        
        pt::read_json(conf_pth, root);
        
        icon_pth = root.get<std::string>("icon-path", "");
        vie = root.get<std::string>("view", "");
        zoom_levl = root.get<std::size_t>("zoom-level", 0);
        compact_layt = root.get<bool>("compact-layout", false);
        *recur = root.get<bool>("apply-recursively", false);
        
        if (!icon_pth.empty())
        {
            if (!apply_gvfs_attribute(dir_pth.c_str(), "metadata::custom-icon", icon_pth.c_str()))
            {
                sucss = false;
            }
        }
        
        if (!vie.empty())
        {
            if (vie == "icons")
            {
                val = "OAFIID:Nemo_File_Manager_Icon_View";
            }
            else
            {
                sucss = false;
                goto end_view;
            }
            
            if (!apply_gvfs_attribute(dir_pth.c_str(), "metadata::nemo-default-view", val))
            {
                sucss = false;
            }
        }
        end_view:
        
        if (zoom_levl != 0)
        {
            std::string str_zoom_levl = std::to_string(zoom_levl);
            if (!apply_gvfs_attribute(dir_pth.c_str(), "metadata::nemo-icon-view-zoom-level",
                                      str_zoom_levl.c_str()))
            {
                sucss = false;
            }
        }
        
        std::string str_compact_layt = compact_layt ? "true" : "false";
        if (!apply_gvfs_attribute(dir_pth.c_str(), "metadata::nemo-icon-view-auto-layout",
                                  str_compact_layt.c_str()))
        {
            sucss = false;
        }
        if (!apply_gvfs_attribute(dir_pth.c_str(), "metadata::nemo-icon-view-tighter-layout",
                                  str_compact_layt.c_str()))
        {
            sucss = false;
        }
    }
    catch (const pt::json_parser::json_parser_error& jpe)
    {
        return false;
    }
    
    return sucss;
}


bool program::apply_gvfs_attribute(const char* fle, const char* attr, const char* val) const
{
    GFile* gfle;
    GFileAttributeType typ;
    gpointer gval;
    GError* err = nullptr;
    
    gfle = g_file_new_for_commandline_arg(fle);
    typ = G_FILE_ATTRIBUTE_TYPE_STRING;
    gval = (void*)val;
    
    if (!g_file_set_attribute(gfle, attr, typ, gval, G_FILE_QUERY_INFO_NONE, nullptr, &err))
    {
        g_error_free(err);
        return false;
    }
    
    return true;
}


}
