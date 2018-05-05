/*
 * This file is part of the KandVST synthesizer.
 *
 * Copyright (c) 2018   Lukas Rahmn, Anton Fredriksson,
 *                      Sarosh Nasir, Stanisław Zwierzchowski,
 *                      Klas Ludvigsson and Andreas Kuszli
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef VST_DEBUG
#include "folders_debug.h"
#define LIB_PATH LIB_PATH_D
#define TEST_PATH TEST_PATH_D
#define TEST_BUILD_PATH TEST_BUILD_PATH_D
#define PY_PATH PY_PATH_D
#elif VST_RELEASE
#include "folders_release.h"
#define LIB_PATH LIB_PATH_R 
#define TEST_PATH TEST_PATH_R
#define PY_PATH PY_PATH_R
#define TEST_BUILD_PATH TEST_BUILD_PATH_R
#else
#error "No vst3 path available for current configuration"
#endif


