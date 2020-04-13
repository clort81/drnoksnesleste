/*
* This file is part of DrNokSnes
*
* Copyright (C) 2005 INdT - Instituto Nokia de Tecnologia
* http://www.indt.org/maemo
* Copyright (C) 2009 Javier S. Pedro <maemo@javispedro.com>
*
* This software is free software; you can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* as published by the Free Software Foundation; either version 2.1 of
* the License, or (at your option) any later version.
*
* This software is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public
* License along with this software; if not, write to the Free Software
* Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA
* 02110-1301 USA
*
*/

#include <string.h>
#include <glib.h>
#include <gio/gio.h>
#include <hildon/hildon-banner.h>
#include <hildon/hildon-file-chooser-dialog.h>

#include "plugin.h"

static gchar * cur_save_uri = NULL;

// Pulling nearly all of gnomevfs just to copy a file. *sigh*.
static gboolean copy_file(const char * source_uri, const char * dest_uri)
{
	GFile* src = g_file_new_for_uri(source_uri);
	GFile* dst = g_file_new_for_uri(dest_uri);
	gboolean res;

	res = g_file_copy (src, dst,
			G_FILE_COPY_TARGET_DEFAULT_PERMS |
			G_FILE_COPY_OVERWRITE,
			NULL, NULL , NULL, NULL);

	g_object_unref(src);
	g_object_unref(dst);

	return res;
}

static gboolean show_result(gboolean res, GtkWindow* parent, const char* msg)
{
	if (res == TRUE) {
		hildon_banner_show_information(GTK_WIDGET(parent), NULL, msg);
		return TRUE;
	} else {
		hildon_banner_show_information(GTK_WIDGET(parent), NULL, "Failed");
		return FALSE;
	}
}

void save_clear()
{
	if (cur_save_uri) {
		g_free(cur_save_uri);
		cur_save_uri = NULL;
	}
}

static gchar * show_dialog(GtkWindow* parent, GtkFileChooserAction action)
{
	GtkWidget * dialog;
	GtkFileFilter * filter;
	gchar * uri = NULL;

	filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter, "*.snsg");

	dialog = hildon_file_chooser_dialog_new_with_properties(GTK_WINDOW(parent), 
		"action", action, "local-only", FALSE, "filter", filter, NULL);

	hildon_file_chooser_dialog_set_extension(HILDON_FILE_CHOOSER_DIALOG(dialog),
		"snsg");
#if defined(MAEMO)
	{
		gchar * games_dir = g_strdup_printf("%s/.games", g_getenv("MYDOCSDIR"));
		gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), games_dir);
		g_free(games_dir);
	}
#endif

	gtk_widget_show_all(GTK_WIDGET(dialog));
	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_OK) {
		uri = gtk_file_chooser_get_uri(GTK_FILE_CHOOSER(dialog));
	}

	gtk_widget_destroy(dialog);

	return uri;
}

void save_load(GtkWindow* parent)
{
	gchar * uri = show_dialog(parent, GTK_FILE_CHOOSER_ACTION_OPEN);

	if (uri) {
		const gchar * frz_file = game_state_get_frz_file();
		gchar * frz_uri = g_filename_to_uri (frz_file, NULL, NULL);
		show_result(copy_file(uri, frz_uri), parent, "Game loaded");
		g_free(frz_uri);
	}

	if (cur_save_uri) {
		g_free(cur_save_uri);
	}
	cur_save_uri = uri;
}

void save_save(GtkWindow* parent)
{
	if (cur_save_uri) {
		const gchar * frz_file = game_state_get_frz_file();
		gchar * frz_uri = g_filename_to_uri (frz_file, NULL, NULL);
		show_result(copy_file(frz_uri, cur_save_uri), parent, "Game saved");
		g_free(frz_uri);
	} else {
		save_save_as(parent);
	}
}

void save_save_as(GtkWindow* parent)
{
	gchar * uri = show_dialog(parent, GTK_FILE_CHOOSER_ACTION_SAVE);

	if (uri) {
		const gchar * frz_file = game_state_get_frz_file();
		gchar * frz_uri = g_filename_to_uri (frz_file, NULL, NULL);
		gboolean res = show_result(copy_file(frz_uri, uri), parent, "Game saved");
		g_free(frz_uri);

		if (!res) return;

		if (cur_save_uri) {
			g_free(cur_save_uri);
		}
		cur_save_uri = uri;
	}
}

