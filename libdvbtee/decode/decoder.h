/*****************************************************************************
 * Copyright (C) 2011-2015 Michael Ira Krufky
 *
 * Author: Michael Ira Krufky <mkrufky@linuxtv.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 *****************************************************************************/

#ifndef __DECODER_H__
#define __DECODER_H__

#include <map>
#if LOCK_DECODER_CHILDREN
#include <pthread.h>
#endif
#include <stdio.h>
#include <stdint.h>

#include "dvbpsi/dvbpsi.h"

namespace dvbtee {

namespace decode {

class Decoder {
public:
	Decoder();
	Decoder(Decoder*);
	virtual ~Decoder();

	int linkChild(Decoder *d);
	bool unlinkChild(int);

protected:
	virtual int getMapIndex() { return -1; }
	Decoder *getParent() { return m_parent; }

private:
	Decoder *m_parent;
#if LOCK_DECODER_CHILDREN
	pthread_mutex_t m_mutex;
#endif
	std::map<int, Decoder*> m_children;

	int linkChild(int, Decoder*);
	int __genMapIdx;
	int genMapIndex();
};

}

}

#endif /* __DECODER_H__ */
