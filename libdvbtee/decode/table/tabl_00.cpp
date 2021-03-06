/*****************************************************************************
 * Copyright (C) 2011-2016 Michael Ira Krufky
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

#include <stdio.h>
#include <stdint.h>
#include "decoder.h"

#include "functions.h"

#include "tabl_00.h"

#define TABLEID 0x00

#define CLASS_MODULE "[PAT]"

#define dPrintf(fmt, arg...) __dPrintf(DBG_DECODE, fmt, ##arg)

using namespace dvbtee::decode;
using namespace valueobj;

static std::string TABLE_NAME = "PAT";

static std::string PATPROGRAM = "PATPROGRAM";

void pat::store(const dvbpsi_pat_t * const p_pat)
#define PAT_DBG 1
{
	if (!p_pat) return;
#if PAT_DBG
	__log_printf(stderr, "%s PAT: v%d, ts_id: %d\n", __func__,
		p_pat->i_version, p_pat->i_ts_id);
#endif
#if 1
	decoded_pat.ts_id   = p_pat->i_ts_id;
	decoded_pat.version = p_pat->i_version;
	decoded_pat.programs.clear();
#endif
	set("tsId", p_pat->i_ts_id);
	set("version", p_pat->i_version);

	Array programs("number");

	const dvbpsi_pat_program_t *p_program = p_pat->p_first_program;
	while (p_program) {
		patProgram *program = new patProgram(this, p_program);
		if (program->isValid()) {
			programs.push((Object*)program);
			//decoded_pat.programs.insert(program->getPair());
		}
#if 1
		decoded_pat.programs[p_program->i_number] = p_program->i_pid;
#endif
		p_program = p_program->p_next;
	}

	set("programs", programs);

	setValid(true);

	dPrintf("%s", toJson().c_str());

	if ((/*changed*/true) && (m_watcher)) {
		m_watcher->updateTable(TABLEID, (Table*)this);
	}
}

patProgram::patProgram(Decoder *parent, const dvbpsi_pat_program_t * const p_program)
: TableDataComponent(parent, PATPROGRAM)
{
	if (!p_program) return;
#if PAT_DBG
	__log_printf(stderr, "  %10d | %x\n",
		p_program->i_number,
		p_program->i_pid);
#endif
	set("number", p_program->i_number);
	set("pid", p_program->i_pid);

	setValid(true);
}

patProgram::~patProgram()
{

}

#if 0
std::pair<uint16_t, uint16_t> patProgram::getPair()
{
	return std::pair<uint16_t, uint16_t>( get<uint16_t>("number"), get<uint16_t>("pid") );
}
#endif

bool pat::ingest(TableStore *s, const dvbpsi_pat_t * const t, TableWatcher *w)
{
	return s->setOnly<const dvbpsi_pat_t, pat>(TABLEID, t, w);
}


pat::pat(Decoder *parent, TableWatcher *watcher)
 : Table(parent, TABLE_NAME, TABLEID, watcher)
{
	//store table later (probably repeatedly)
}

pat::pat(Decoder *parent, TableWatcher *watcher, const dvbpsi_pat_t * const p_pat)
 : Table(parent, TABLE_NAME, TABLEID, watcher)
{
	store(p_pat);
}

pat::~pat()
{
	//
}

REGISTER_TABLE_FACTORY(TABLEID, const dvbpsi_pat_t, pat)
