/* First, the standard lib includes, alphabetically ordered */
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

/* Then, this project's includes, alphabetically ordered */
#include "../inc/dict.h"
#include "../inc/dict_helpers.h"
#include "../inc/helpers.h"
#include "../inc/string.h"

#define ADD 'a'
#define REMOVE 'r'
#define DUMP 'u'
#define EMPTY 'e'
#define LOAD 'l'
#define SEARCH 's'
#define REPLACE 'c'
#define SHOW 'h'
#define SIZE 'z'
#define QUIT 'q'

#define RESULT_PREFIX "\t-> "

void print_help(char *program_name)
{
    /* Print the usage help of this program. */
    printf("Usage: %s\n\n", program_name);
}

void print_hello(GtkWidget *widget,
                 gpointer data)
{
    g_print("hola mundo");
}

void on_test_handler(GtkWidget *widget,
                     gpointer data)
{
    g_print("hola test");
    GtkTextView *textview = GTK_TEXT_VIEW(data);
    GtkTextBuffer *textbuffer = GTK_TEXT_BUFFER(gtk_text_view_get_buffer(textview));
    const gchar *new_text = "Nuevo contenido para el TextView\n";
    GtkTextIter *iter;
    gtk_text_buffer_set_text(textbuffer,new_text,-1);
}

static void
activate(GtkApplication *app,
         gpointer user_data)
{
    GtkWidget *window;
    GtkWidget *m_vbox;

    GtkWidget *buttons_container;
    GtkWidget *textviews_box;

    GtkWidget *word_textview;
    GtkWidget *def_textview;
    GtkWidget *m_separator;

    GtkWidget *button_creator;

    /* create a new window, and set its title */
    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Dictionary");
    gtk_container_set_border_width(GTK_CONTAINER(window), 10);
    gtk_widget_set_size_request(window, 800, 600);

    // Crear contenedor vertical principal
    m_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    gtk_container_add(GTK_CONTAINER(window), m_vbox);

    // Contenedores del texto
    GtkWidget *scroll_wind_defs = gtk_scrolled_window_new(NULL, NULL);  // Hay que agregar un contenedor para poder scrollear
    GtkWidget *scroll_wind_words = gtk_scrolled_window_new(NULL, NULL); // Hay que agregar un contenedor para poder scrollear

    textviews_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_start(GTK_BOX(m_vbox), textviews_box, TRUE, TRUE, 10);

    //
    word_textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(word_textview), FALSE);
    gtk_container_add(GTK_CONTAINER(scroll_wind_words), word_textview);
    gtk_box_pack_start(GTK_BOX(textviews_box), scroll_wind_words, TRUE, TRUE, 10);
    // ESTILOS?
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(word_textview), 20);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(word_textview), FALSE);
    //
    m_separator = gtk_separator_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_end(GTK_BOX(textviews_box), m_separator, FALSE, FALSE, 10);
    //
    def_textview = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(def_textview), FALSE);
    gtk_container_add(GTK_CONTAINER(scroll_wind_defs), def_textview);
    gtk_box_pack_end(GTK_BOX(textviews_box), scroll_wind_defs, TRUE, TRUE, 10);
    // ESTILOS?
    gtk_text_view_set_left_margin(GTK_TEXT_VIEW(def_textview), 20);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(def_textview), FALSE);

    // buffers para el texto
    GtkTextBuffer *word_buffer;
    word_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(word_textview));
    gtk_text_buffer_set_text(word_buffer, "hola", 4);

    GtkTextBuffer *def_buffer;
    def_buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(def_textview));
    gtk_text_buffer_set_text(def_buffer, "hello", 5);

    // Botonera
    buttons_container = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
    gtk_box_pack_end(GTK_BOX(m_vbox), buttons_container, FALSE, TRUE, 10);
    gtk_widget_set_size_request(buttons_container, 500, 10);

    button_creator = gtk_button_new_with_label("Add definition");
    g_signal_connect(button_creator, "clicked", G_CALLBACK(on_test_handler), word_textview);
    gtk_box_pack_start(GTK_BOX(buttons_container), button_creator, TRUE, TRUE, 2);

    button_creator = gtk_button_new_with_label("Empty dict");
    g_signal_connect(button_creator, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_box_pack_start(GTK_BOX(buttons_container), button_creator, TRUE, TRUE, 2);

    button_creator = gtk_button_new_with_label("Remove definition");
    g_signal_connect(button_creator, "clicked", G_CALLBACK(print_hello), NULL);
    gtk_box_pack_start(GTK_BOX(buttons_container), button_creator, TRUE, TRUE, 2);

    gtk_widget_show_all(window);
}

char print_menu(void)
{
    char result = '\0';
    char *line = NULL;

    printf("\nChoose what you want to do. Options are:\n"
           "\n"
           "\t**************************************************************\n"
           "\t* z: Size of the dictionary                                  *\n"
           "\t* s: Search for a definition in the dict                     *\n"
           "\t* a: Add a new word to the dict                              *\n"
           "\t* r: Remove a word from the dict                             *\n"
           "\t* c: Change a definition to the dict                         *\n"
           "\t* e: Empty the dict                                          *\n"
           "\t* h: Show the dict in stdout                                 *\n"
           "\t* l: Load the dict from a file                               *\n"
           "\t* u: Dump the dict to a file                                 *\n"
           "\t* q: Quit                                                    *\n"
           "\t**************************************************************\n"
           "\nPlease enter your choice: ");

    line = readline_from_stdin();
    if (line != NULL)
    {
        result = line[0];
    }

    free(line);
    return (result);
}

bool is_valid_option(char option)
{
    bool result = false;

    result = (option == ADD || option == REMOVE ||
              option == DUMP || option == EMPTY || option == LOAD ||
              option == SEARCH || option == SHOW || option == SIZE ||
              option == QUIT);

    return (result);
}

string get_input(const char *message)
{
    string result = NULL;
    printf("\t%s: ", message);
    result = readstring_from_stdin();
    assert(result != NULL);
    return (result);
}

dict_t on_add(dict_t current)
{
    string definition = NULL;
    string word = NULL;
    word = get_input("Please enter the word to add into the dict");
    if (dict_exists(current, word))
    {
        printf(RESULT_PREFIX "The word is already in the dict.\n");
        word = string_destroy(word);
    }
    else
    {
        definition = get_input("Please enter the definition");
        current = dict_add(current, word, definition);
        printf(RESULT_PREFIX "The word and definition were added.\n");
    }
    return (current);
}

dict_t on_replace(dict_t current)
{
    string definition = NULL;
    string word = NULL;
    word = get_input("Please enter the word to replace in the dict");
    if (!dict_exists(current, word))
    {
        printf(RESULT_PREFIX "The word does not exist in the dict.\n");
        word = string_destroy(word);
    }
    else
    {
        definition = get_input("Please enter the new definition");
        current = dict_add(current, word, definition);
        printf(RESULT_PREFIX "The definition was replaced.\n");
    }
    return (current);
}

dict_t on_remove(dict_t current)
{
    string word =
        get_input("Please enter the word to delete from the dict");
    if (!dict_exists(current, word))
    {
        printf(RESULT_PREFIX "The word does not exist in the dict.\n");
    }
    else
    {
        current = dict_remove(current, word);
        printf(RESULT_PREFIX "The word was removed.\n");
    }
    word = string_destroy(word);
    return (current);
}

dict_t on_load(dict_t current)
{
    string filename = get_input("Please enter the filename to load the dict from");
    dict_t other = dict_from_file(filename);
    if (other == NULL)
    {
        printf("Can not load dict from filename %s\n", string_ref(filename));
    }
    else
    {
        printf(RESULT_PREFIX "The dictionary was successfully loaded.\n");
        current = dict_destroy(current); /* destroy current */
        current = other;
    }
    filename = string_destroy(filename);
    return (current);
}

void on_dump(dict_t current)
{
    string filename = get_input("Please enter the filename to dump the file");
    dict_to_file(current, filename);
    filename = string_destroy(filename);
    printf(RESULT_PREFIX "The dictionary was successfully dumped.\n");
}

dict_t on_empty(dict_t current)
{
    current = dict_remove_all(current);
    printf(RESULT_PREFIX "All words were removed\n");
    return (current);
}

void on_search(dict_t current)
{
    string definition = NULL;
    string word = NULL;
    word = get_input("Please enter the word to search in the dict");
    definition = dict_search(current, word);
    if (definition == NULL)
    {
        printf(RESULT_PREFIX "The word does not exist in the dict\n");
    }
    else
    {
        printf(RESULT_PREFIX "The definition of \"%s\" is : \"%s\"\n",
               string_ref(word), string_ref(definition));
    }
    word = string_destroy(word);
}

void on_size(dict_t current)
{
    printf(RESULT_PREFIX "The size of the dict is %u\n", dict_length(current));
}

int main(int argc,
         char **argv)
{
    char option = '\0';
    dict_t current = dict_empty();
    /* print a simple menu and do the requested operations */

    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);

    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);

    return status;
}