/*
 * This file is part of EasyRPG Player.
 *
 * EasyRPG Player is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * EasyRPG Player is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with EasyRPG Player. If not, see <http://www.gnu.org/licenses/>.
 */

// Headers
#include <string>
#include <iomanip>
#include <sstream>
#include "window_savefile.h"
#include "game_system.h"
#include "input.h"
#include "bitmap.h"
#include "font.h"
#include "player.h"

Window_SaveFile::Window_SaveFile(int ix, int iy, int iwidth, int iheight) :
	Window_Base(ix, iy, iwidth, iheight),
	index(0), hero_hp(0), hero_level(0), corrupted(false), has_save(false) {

	SetBorderX(4);
	SetContents(Bitmap::Create(width - 8, height - 16));

	Refresh();
	UpdateCursorRect();
}

void Window_SaveFile::UpdateCursorRect() {
	Rect rect = Rect();

	if (GetActive()) {
		std::ostringstream out;
		out << Data::terms.file << std::setw(2) << std::setfill(' ') << index + 1;
		rect = Rect(0, 0, Font::Default()->GetSize(out.str()).width + 6, 16);
	}

	SetCursorRect(rect);
}

void Window_SaveFile::SetIndex(int id) {
	index = id;
}

void Window_SaveFile::SetParty(const std::vector<std::pair<int, std::string> >& actors,
	std::string name, int hp, int level) {
	party = actors;
	hero_name = name;
	hero_hp = hp;
	hero_level = level;
}

void Window_SaveFile::SetCorrupted(bool corrupted) {
	this->corrupted = corrupted;
}

bool Window_SaveFile::IsValid() {
	return has_save && !corrupted;
}

void Window_SaveFile::SetHasSave(bool valid) {
	this->has_save = valid;
}

void Window_SaveFile::Refresh() {
	contents->Clear();

	std::ostringstream out;
	out << Data::terms.file << std::setw(2) << std::setfill(' ') << index + 1;
	contents->TextDraw(4, 2, has_save ? Font::ColorDefault : Font::ColorDisabled, out.str());

	if (corrupted) {
		contents->TextDraw(4, 16 + 2, Font::ColorKnockout, "Savegame corrupted");
		return;
	}

	if (party.empty())
		return;


	contents->TextDraw(4, 16 + 2, Font::ColorDefault, hero_name);
	auto lvl_short = Data::terms.lvl_short;
	if (lvl_short.size() != 2) {
		lvl_short.resize(2, ' ');
	}

	contents->TextDraw(4, 32 + 2, 1, lvl_short);

	int lx = Font::Default()->GetSize(lvl_short).width;
	out.str("");
	out << std::setw(2) << std::setfill(' ') << hero_level;
	contents->TextDraw(4 + lx, 32 + 2, Font::ColorDefault, out.str());

	auto hp_short = Data::terms.hp_short;
	if (hp_short.size() != 2) {
		hp_short.resize(2, ' ');
	}

	contents->TextDraw(46, 32 + 2, 1, hp_short);

	int hx = Font::Default()->GetSize(hp_short).width;
	out.str("");
	out << std::setw(Player::IsRPG2k3() ? 4 : 3) << std::setfill(' ') << hero_hp;
	contents->TextDraw(46 + hx, 32 + 2, Font::ColorDefault, out.str());

	for (int i = 0; i < 4 && (size_t) i < party.size(); i++) {
		DrawFace(party[i].second, party[i].first, 92 + i * 56, 0);
	}
}

void Window_SaveFile::Update() {
	Window_Base::Update();
	UpdateCursorRect();
}
