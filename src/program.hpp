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

#ifndef DIRCONF_PROGRAM_HPP
#define DIRCONF_PROGRAM_HPP

#include <filesystem>
#include <vector>

#include <speed/speed.hpp>

#include "file_managers.hpp"


namespace dirconf {


class program
{
public:
    program(
            std::filesystem::path src_pth,
            std::vector<std::string> conf_nmes,
            spdcontain::flags<file_managers> fle_managrs
    );
    
    int execute() const;
    
private:
    bool execute_in_directory(
            const std::filesystem::path& dir_pth,
            const std::filesystem::path& last_conf_pth
    ) const;
    
    bool apply_configuration(
            const std::filesystem::path& dir_pth,
            const std::filesystem::path& conf_pth,
            bool* recur
    ) const;
    
    bool apply_gvfs_attribute(const char* fle, const char* attr, const char* val) const;
    
private:
    std::filesystem::path src_pth_;
    
    std::vector<std::string> conf_nmes_;
    
    spdcontain::flags<file_managers> fle_managrs_;
};


}


#endif
