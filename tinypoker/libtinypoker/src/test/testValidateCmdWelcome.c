/*
 * Copyright (C) 2007 Thomas Cort <code@member.fsf.org>
 *
 * This file is part of libtinypoker.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "../main/tinypoker.h"
#include "test.h"

int main() {
	assertTrue("Welcome String should be valid", ipp_validate_msg(REGEX_MSG_WELCOME, "WELCOME BAGELBOT"));
	assertTrue("Welcome String should be valid", ipp_validate_msg(REGEX_MSG_WELCOME, "WELCOME Coat_Hanger"));

	assertFalse("Welcome String should not be valid", ipp_validate_msg(REGEX_MSG_WELCOME, ""));
	assertFalse("Welcome String should not be valid", ipp_validate_msg(REGEX_MSG_WELCOME, "WELCOME "));
	assertFalse("Welcome String should not be valid", ipp_validate_msg(REGEX_MSG_WELCOME, "WELCOME\n"));
	assertFalse("Welcome String should not be valid", ipp_validate_msg(REGEX_MSG_WELCOME, "WELCOME \n"));
	assertFalse("Welcome String should not be valid", ipp_validate_msg(REGEX_MSG_WELCOME, "WELCOME X 1 1"));
	return PASS;
}
