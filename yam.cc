// main.cc
#include <gtk/gtk.h>
#include "webview/webview.h"
#include <libappindicator/app-indicator.h>

extern const guint8 tray_icon[];

static void
exit_clicked_cb (GtkWidget *widget, gpointer data) {
    gtk_main_quit();
}

static void
show_clicked_cb (GtkWidget *widget, gpointer data) {
    gtk_widget_show(reinterpret_cast<GtkWidget*>(data));
}

gboolean
on_widget_deleted(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_widget_hide(widget);
    return TRUE;
}

int main(int argc, char** argv) {

    gtk_init (&argc, &argv);

    GtkWidget *window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), "GtkStatusIcon Example");
    gtk_widget_set_size_request (window, 1024, 768);

    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_resource ("/com/example/mytest/icon", NULL);
    GError* e = NULL;
    gdk_pixbuf_save(pixbuf, "/tmp/yandex-music-icon.png","png", &e, NULL);

    gtk_window_set_icon (GTK_WINDOW(window), pixbuf);

    g_signal_connect(G_OBJECT(window),
        "delete-event", G_CALLBACK(on_widget_deleted), NULL);

    AppIndicator *indicator;

    indicator = app_indicator_new ("example-simple-client",
                                 "indicator-messages",
                                 APP_INDICATOR_CATEGORY_APPLICATION_STATUS);

    app_indicator_set_status(indicator, APP_INDICATOR_STATUS_ACTIVE);
    app_indicator_set_icon(indicator, "/tmp/yandex-music-icon.png");


    GtkWidget *menu = NULL;
    GtkWidget *item = NULL;

    menu = gtk_menu_new ();

    item = gtk_menu_item_new_with_label ("Show");
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);

    g_signal_connect (item, "activate",
        G_CALLBACK (show_clicked_cb), reinterpret_cast<gpointer>(window));

    gtk_widget_show (item);

    item = gtk_menu_item_new_with_label ("Exit");
    gtk_menu_shell_append (GTK_MENU_SHELL (menu), item);

    g_signal_connect (item, "activate",
        G_CALLBACK (exit_clicked_cb), reinterpret_cast<gpointer>(window));

    gtk_widget_show (item);

    app_indicator_set_menu (indicator, GTK_MENU (menu));

    webview::webview w(true, window);

    w.set_title("Yandex Music");
    w.navigate("https://passport.yandex.com/auth?origin=music_button-header&retpath=https%3A%2F%2Fmusic.yandex.com%2Fhome");
    w.run();
    return 0;
}
