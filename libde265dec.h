/*
 * H.265 decoder
 *
 * Copyright (c) 2013, Dirk Farin <dirk.farin@gmail.com>
 * Copyright (c) 2013-2015, Joachim Bauch <bauch@struktur.de>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

/**
 * @file
 * H.265 decoder based on libde265
 */

#ifndef LIBDE265DEC_H
#define LIBDE265DEC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <libavcodec/avcodec.h>

extern AVCodec ff_libde265_decoder;

void libde265dec_register(void);

#ifdef __cplusplus
}
#endif

#endif  // LIBDE265DEC_H
