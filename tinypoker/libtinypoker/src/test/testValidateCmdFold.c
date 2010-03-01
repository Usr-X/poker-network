/*
 * Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010 Thomas Cort <linuxgeek@gmail.com>
 *
 * This file is part of TinyPoker.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "../main/tinypoker.h"
#include "test.h"

int main()
{
	ipp_init();

	assertTrue("Fold String should be valid", ipp_validate_msg(REGEX_MSG_FOLD, "FOLD"));

	assertFalse("Fold String should not be valid", ipp_validate_msg(REGEX_MSG_FOLD, "FOL"));
	assertFalse("Fold String should not be valid", ipp_validate_msg(REGEX_MSG_FOLD, "FOLD?"));
	assertFalse("Fold String should not be valid", ipp_validate_msg(REGEX_MSG_FOLD, "FLOD"));
	assertFalse("Fold String should not be valid", ipp_validate_msg(REGEX_MSG_FOLD, "FOLD "));
	assertFalse("Fold String should not be valid", ipp_validate_msg(REGEX_MSG_FOLD, ""));

	ipp_exit();
	return PASS;
}
