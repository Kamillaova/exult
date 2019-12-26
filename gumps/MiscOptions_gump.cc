/*
 *  Copyright (C) 2001-2013  The Exult Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <iostream>
#include <cstring>

#include "SDL_events.h"

#include "Gump_manager.h"
#include "Configuration.h"
#include "Gump_button.h"
#include "Gump_ToggleButton.h"
#include "MiscOptions_gump.h"
#include "exult.h"
#include "exult_flx.h"
#include "gamewin.h"
#include "combat_opts.h"
#include "Text_button.h"
#include "Enabled_button.h"
#include "font.h"
#include "gamewin.h"
#include "Notebook_gump.h"
#include "ShortcutBar_gump.h"
using std::string;

static const int rowy[] = { 4, 16, 28, 40, 52, 64, 76, 88, 100, 112, 124, 136, 148, 160, 172 };
static const int colx[] = { 35, 50, 120, 170, 192, 211 };

static const char *oktext = "OK";
static const char *canceltext = "CANCEL";

using MiscOptions_button = CallbackTextButton<MiscOptions_gump>;
using MiscTextToggle = CallbackToggleTextButton<MiscOptions_gump>;
using MiscEnabledToggle = CallbackEnabledButton<MiscOptions_gump>;

void MiscOptions_gump::close() {
	save_settings();
	done = true;
}

void MiscOptions_gump::cancel() {
	done = true;
}

void MiscOptions_gump::build_buttons() {
	string *yesNo1 = new string[2]; // TODO:need to make this like enabled
	yesNo1[0] = "No";              // if I am going to add much more
	yesNo1[1] = "Yes";
	string *yesNo2 = new string[2];
	yesNo2[0] = "No";
	yesNo2[1] = "Yes";
	string *yesNo3 = new string[2];
	yesNo3[0] = "No";
	yesNo3[1] = "Yes";
	std::string *diffs = new std::string[7];
	diffs[0] = "Easiest (-3)";
	diffs[1] = "Easier (-2)";
	diffs[2] = "Easier (-1)";
	diffs[3] = "Normal";
	diffs[4] = "Harder (+1)";
	diffs[5] = "Harder (+2)";
	diffs[6] = "Hardest (+3)";
	std::string *stacks_text = new std::string[2];
	stacks_text[0] = "No";
	stacks_text[1] = "Yes";
	std::string *autonotes_text = new std::string[2];
	autonotes_text[0] = "No";
	autonotes_text[1] = "Yes";

	string *sc_enabled_txt = new string[3];
	sc_enabled_txt[0] = "No";
	sc_enabled_txt[1] = "transparent";
	sc_enabled_txt[2] = "Yes";

	sc_outline_txt = new string[8]; // keep in order of Pixel_colors
	sc_outline_txt[0] = "green";
	sc_outline_txt[1] = "white";
	sc_outline_txt[2] = "yellow";
	sc_outline_txt[3] = "blue";
	sc_outline_txt[4] = "red";
	sc_outline_txt[5] = "purple";
	sc_outline_txt[6] = "black";
	sc_outline_txt[7] = "No"; // needs to be last

	string *yesNo4 = new string[2];
	yesNo4[0] = "No";
	yesNo4[1] = "Yes";

	int y_index = 0;
	int small_size = 44;
	int large_size = 85;
	buttons[id_scroll_mouse] = new MiscTextToggle(this, &MiscOptions_gump::toggle_scroll_mouse,
	        yesNo1, scroll_mouse, 2, colx[5], rowy[y_index], small_size);
	buttons[id_menu_intro] = new MiscTextToggle(this, &MiscOptions_gump::toggle_menu_intro,
	        yesNo2, menu_intro, 2, colx[5], rowy[++y_index], small_size);
	buttons[id_usecode_intro] = new MiscTextToggle(this, &MiscOptions_gump::toggle_usecode_intro,
	        yesNo3, usecode_intro, 2, colx[5], rowy[++y_index], small_size);
	buttons[id_alternate_drop] = new MiscTextToggle(this, &MiscOptions_gump::toggle_alternate_drop,
	        stacks_text, alternate_drop, 2, colx[5], rowy[++y_index], small_size);
	buttons[id_allow_autonotes] = new MiscTextToggle(this, &MiscOptions_gump::toggle_allow_autonotes,
	        autonotes_text, allow_autonotes, 2, colx[5], rowy[++y_index], small_size);
	buttons[id_sc_enabled] = new MiscTextToggle(this, &MiscOptions_gump::toggle_sc_enabled,
	        sc_enabled_txt, sc_enabled, 3, colx[3], rowy[++y_index], large_size);
	buttons[id_sc_outline] = new MiscTextToggle(this, &MiscOptions_gump::toggle_sc_outline,
	        sc_outline_txt, sc_outline, 8, colx[5], rowy[++y_index], small_size);
	buttons[id_sb_hide_missing] = new MiscTextToggle(this, &MiscOptions_gump::toggle_sb_hide_missing,
	        yesNo4, sb_hide_missing, 2, colx[5], rowy[++y_index], small_size);
	// two row gap
	buttons[id_difficulty] = new MiscTextToggle(this, &MiscOptions_gump::toggle_difficulty,
	        diffs, difficulty, 7, colx[3], rowy[y_index+=3], large_size);
	buttons[id_show_hits] = new MiscEnabledToggle(this, &MiscOptions_gump::toggle_show_hits,
	        show_hits, colx[3], rowy[++y_index], large_size);

	std::string *modes = new std::string[2];
	modes[0] = "Original";
	modes[1] = "Space pauses";
	buttons[id_mode] = new MiscTextToggle(this, &MiscOptions_gump::toggle_mode,
	        modes, mode, 2, colx[3], rowy[++y_index], large_size);
	std::string *charmedDiff = new std::string[2];
	charmedDiff[0] = "Normal";
	charmedDiff[1] = "Hard";
	buttons[id_charmDiff] = new MiscTextToggle(this, &MiscOptions_gump::toggle_charmDiff,
	        charmedDiff, charmDiff, 2, colx[3], rowy[++y_index], large_size);
	// Ok
	buttons[id_ok] = new MiscOptions_button(this, &MiscOptions_gump::close,
	        oktext, colx[0], rowy[++y_index]);
	// Cancel
	buttons[id_cancel] = new MiscOptions_button(this, &MiscOptions_gump::cancel,
	        canceltext, colx[4], rowy[y_index]);
}

void MiscOptions_gump::load_settings() {
	string yn;
	scroll_mouse = gwin->can_scroll_with_mouse();
	config->value("config/gameplay/skip_intro", yn, "no");
	usecode_intro = (yn == "yes");
	config->value("config/gameplay/skip_splash", yn, "no");
	menu_intro = (yn == "yes");

	sc_enabled = gwin->get_shortcutbar_type();
	sc_outline = gwin->get_outline_color();
	sb_hide_missing = gwin->sb_hide_missing_items();

	difficulty = Combat::difficulty;
	if (difficulty < -3)
		difficulty = -3;
	else if (difficulty > 3)
		difficulty = 3;
	difficulty += 3;        // Scale to choices (0-6).
	show_hits = Combat::show_hits ? 1 : 0;
	mode = static_cast<int>(Combat::mode);
	if (mode < 0 || mode > 1)
		mode = 0;
	charmDiff = Combat::charmed_more_difficult;
	alternate_drop = gwin->get_alternate_drop();
	allow_autonotes = gwin->get_allow_autonotes();
}

MiscOptions_gump::MiscOptions_gump()
	: Modal_gump(nullptr, EXULT_FLX_MISCOPTIONS_SHP, SF_EXULT_FLX) {
	set_object_area(Rectangle(0, 0, 0, 0), 8, 184);//++++++ ???

	load_settings();
	build_buttons();
}

MiscOptions_gump::~MiscOptions_gump() {
	for (int i = id_first; i < id_count; i++)
		if (buttons[i])
			delete buttons[i];
}

void MiscOptions_gump::save_settings() {
	config->set("config/gameplay/scroll_with_mouse",
	            scroll_mouse ? "yes" : "no", false);
	gwin->set_mouse_with_scroll(scroll_mouse);
	config->set("config/gameplay/skip_intro",
	            usecode_intro ? "yes" : "no", false);
	config->set("config/gameplay/skip_splash",
	            menu_intro ? "yes" : "no", false);

	string str = "no";
	if(sc_enabled == 1)
		str = "transparent";
	else if(sc_enabled == 2)
		str = "yes";
	config->set("config/shortcutbar/use_shortcutbar", str, false);
	config->set("config/shortcutbar/use_outline_color", sc_outline_txt[sc_outline], false);
	config->set("config/shortcutbar/hide_missing_items", sb_hide_missing ? "yes" : "no", false);

	gwin->set_outline_color(static_cast<Pixel_colors>(sc_outline));
	gwin->set_sb_hide_missing_items(sb_hide_missing);
	gwin->set_shortcutbar(static_cast<uint8>(sc_enabled));
	if(g_shortcutBar)
		g_shortcutBar->set_changed();

	Combat::difficulty = difficulty - 3;
	config->set("config/gameplay/combat/difficulty",
	            Combat::difficulty, false);
	Combat::show_hits = (show_hits != 0);
	config->set("config/gameplay/combat/show_hits",
	            show_hits ? "yes" : "no", false);
	Combat::mode = static_cast<Combat::Mode>(mode);
	str = Combat::mode == Combat::keypause ? "keypause"
	                  : "original";
	config->set("config/gameplay/combat/mode", str, false);
	Combat::charmed_more_difficult = charmDiff;
	config->set("config/gameplay/combat/charmDifficulty",
	            charmDiff ? "hard" : "normal", false);
	gwin->set_alternate_drop(alternate_drop);
	config->set("config/gameplay/alternate_drop",
	            alternate_drop ? "yes" : "no", false);
	gwin->set_allow_autonotes(allow_autonotes);
	config->set("config/gameplay/allow_autonotes",
	            allow_autonotes ? "yes" : "no", false);
	config->write_back();
}

void MiscOptions_gump::paint() {
	Gump::paint();
	for (int i = id_first; i < id_count; i++)
		if (buttons[i])
			buttons[i]->paint();
	Font *font = fontManager.get_font("SMALL_BLACK_FONT");
	Image_window8 *iwin = gwin->get_win();
	int y_index = 0;
	font->paint_text(iwin->get_ib8(), "Scroll game view with mouse:", x + colx[0], y + rowy[y_index] + 1);
	font->paint_text(iwin->get_ib8(), "Skip intro:", x + colx[0], y + rowy[++y_index] + 1);
	font->paint_text(iwin->get_ib8(), "Skip scripted first scene:", x + colx[0], y + rowy[++y_index] + 1);
	font->paint_text(iwin->get_ib8(), "Alternate drag'n'drop:", x + colx[0], y + rowy[++y_index] + 1);
	font->paint_text(iwin->get_ib8(), "Allow Autonotes:", x + colx[0], y + rowy[++y_index] + 1);
	font->paint_text(iwin->get_ib8(), "Use ShortcutBar :", x + colx[0], y + rowy[++y_index] + 1);
	font->paint_text(iwin->get_ib8(), "Use outline color :", x + colx[1], y + rowy[++y_index] + 1);
	font->paint_text(iwin->get_ib8(), "Hide missing items:", x + colx[1], y + rowy[++y_index] + 1);
	// 1 row gap
	font->paint_text(iwin->get_ib8(), "Combat Options:", x + colx[0], y + rowy[y_index+=2] + 1);
	font->paint_text(iwin->get_ib8(), "Difficulty:", x + colx[1], y + rowy[++y_index] + 1);
	font->paint_text(iwin->get_ib8(), "Show Hits:", x + colx[1], y + rowy[++y_index] + 1);
	font->paint_text(iwin->get_ib8(), "Mode:", x + colx[1], y + rowy[++y_index] + 1);
	font->paint_text(iwin->get_ib8(), "Charmed Difficulty:", x + colx[1], y + rowy[++y_index] + 1);
	gwin->set_painted();
}

bool MiscOptions_gump::mouse_down(int mx, int my, int button) {
	// Only left and right buttons
	if (button != 1 && button != 3) return false;

	// We'll eat the mouse down if we've already got a button down
	if (pushed) return true;

	// First try checkmark
	pushed = Gump::on_button(mx, my);

	// Try buttons at bottom.
	if (!pushed) {
		for (int i = id_first; i < id_count; i++) {
			if (buttons[i] && buttons[i]->on_button(mx, my)) {
				pushed = buttons[i];
				break;
			}
		}
	}

	if (pushed && !pushed->push(button))            // On a button?
		pushed = nullptr;

	return button == 1 || pushed != nullptr;
}

bool MiscOptions_gump::mouse_up(int mx, int my, int button) {
	// Not Pushing a button?
	if (!pushed) return false;

	if (pushed->get_pushed() != button) return button == 1;

	bool res = false;
	pushed->unpush(button);
	if (pushed->on_button(mx, my))
		res = pushed->activate(button);
	pushed = nullptr;
	return res;
}
