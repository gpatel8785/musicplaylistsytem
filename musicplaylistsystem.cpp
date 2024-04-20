#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes for validation
int isValidPlaylistName(const char* name);
int isValidSongTitle(const char* title);
int isValidArtistName(const char* artist);
int isValidDuration(int duration);

typedef struct Song
{
    char title[100];
    char artist[100];
    int duration;
    struct Song* next;
} Song;

typedef struct Playlist
{
    char name[100];
    Song* head;
    struct Playlist* next;
} Playlist;

#define HASH_SIZE 100
typedef struct HashTable
{
    Playlist* table[HASH_SIZE];
} HashTable;

unsigned int hash(const char* key)
{
    unsigned int hashVal = 0;
    for (size_t i = 0; key[i] != '\0'; i++)
    {
        hashVal = key[i] + (hashVal << 5) - hashVal;
    }
    return hashVal % HASH_SIZE;
}

void insertPlaylist(HashTable* table, Playlist* playlist)
{
    unsigned int index = hash(playlist->name);
    playlist->next = table->table[index];
    table->table[index] = playlist;
    
}

Playlist* searchPlaylist(HashTable* table, const char* name)
{
    unsigned int index = hash(name);
    Playlist* current = table->table[index];
    
         return current;
}

void searchPlaylistsBySong(HashTable* table, const char* songTitle)
{
    int found = 0;
    for (int i = 0; i < HASH_SIZE; i++)
    {
        Playlist* current = table->table[i];
        while (current != NULL)
        {
            Song* currentSong = current->head;
            while (currentSong != NULL)
            {
                if (strcmp(currentSong->title, songTitle) == 0)
                {
                    if (!found)
                    {
                        printf("Playlists containing the song '%s':\n", songTitle);
                        found = 1;
                    }
                    printf("- %s\n", current->name);
                    break;
                }
                currentSong = currentSong->next;
            }
            current = current->next;
        }
    }
    if (!found)
    {
        printf("No playlists found containing the song '%s'.\n", songTitle);
    }
}

Song* createSong(const char* title, const char* artist, int duration)
{
    Song* newSong = (Song*)malloc(sizeof(Song));
    if (newSong)
    {
        strncpy_s(newSong->title, title, sizeof(newSong->title) - 1);
        newSong->title[sizeof(newSong->title) - 1] = '\0';
        strncpy_s(newSong->artist, artist, sizeof(newSong->artist) - 1);
        newSong->artist[sizeof(newSong->artist) - 1] = '\0';
        newSong->duration = duration;
        newSong->next = NULL;
    }
    return newSong;
}

void addSong(Playlist* playlist, const char* title, const char* artist, int duration)
{
    Song* newSong = createSong(title, artist, duration);
    if (newSong)
    {
        if (playlist->head == NULL)
        {
            playlist->head = newSong;
        }
        else
        {
            Song* current = playlist->head;
            while (current->next != NULL)
            {
                current = current->next;
            }
            current->next = newSong;
        }
    }
}

void displayPlaylist(Playlist* playlist)
{
    printf("Playlist: %s\n", playlist->name);
    Song* current = playlist->head;
    int count = 1;
    while (current != NULL)
    {
        printf("%d. Title: %s, Artist: %s, Duration: %d sec\n", count, current->title, current->artist, current->duration);
        current = current->next;
        count++;
    }
}

int main()
{
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    memset(table, 0, sizeof(HashTable)); 

    char line[1024];  

    while (1)
    {
        printf("\n1. Create Playlist\n2. Search Playlist\n3. Add Song to Playlist\n4. Display Playlist\n5. Search Playlists by Song\n6. Exit\nEnter your choice: ");
        if (!fgets(line, sizeof(line), stdin))
        {
            printf("Error reading input.\n");
            continue;
        }
        line[strcspn(line, "\n")] = 0;  

        int choice;
        if (sscanf_s(line, "%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice)
        {
        case 1:
        {
            printf("Enter playlist name: ");
            if (!fgets(line, sizeof(line), stdin))
            {
                printf("Error reading input.\n");
                continue;
            }
            line[strcspn(line, "\n")] = 0;  

            Playlist* newPlaylist = (Playlist*)malloc(sizeof(Playlist));
            if (newPlaylist)
            {
                strncpy_s(newPlaylist->name, line, sizeof(newPlaylist->name));
                newPlaylist->name[sizeof(newPlaylist->name) - 1] = '\0';
                newPlaylist->head = NULL;
                newPlaylist->next = NULL;
                insertPlaylist(table, newPlaylist);
                printf("Playlist '%s' created successfully.\n", line);
            }
            else
            {
                printf("Memory allocation failed.\n");
            }
            break;
        }
        case 2:
        {
            printf("Enter playlist name: ");
            if (!fgets(line, sizeof(line), stdin))
            {
                printf("Error reading input.\n");
                continue;
            }
            line[strcspn(line, "\n")] = 0;  
            Playlist* foundPlaylist = searchPlaylist(table, line);
            if (foundPlaylist)
            {
                printf("Playlist found: %s\n", foundPlaylist->name);
            }
            else
            {
                printf("Playlist not found.\n");
            }
            break;
        }
        case 3:
        {
            char playlistName[100], title[100], artist[100];
            int duration;

            printf("Enter playlist name: ");
            if (!fgets(playlistName, sizeof(playlistName), stdin))
            {
                printf("Error reading input.\n");
                continue;
            }
            playlistName[strcspn(playlistName, "\n")] = 0;  

            Playlist* foundPlaylist = searchPlaylist(table, playlistName);
            if (!foundPlaylist)
            {
                printf("Playlist not found.\n");
                continue;
            }

            printf("Enter song title: ");
            if (!fgets(title, sizeof(title), stdin))
            {
                printf("Error reading input.\n");
                continue;
            }
            title[strcspn(title, "\n")] = 0;  

            printf("Enter artist name: ");
            if (!fgets(artist, sizeof(artist), stdin))
            {
                printf("Error reading input.\n");
                continue;
            }
            artist[strcspn(artist, "\n")] = 0; 

            printf("Enter duration (in seconds): ");
            if (!fgets(line, sizeof(line), stdin))
            {
                printf("Error reading input.\n");
                continue;
            }
            if (sscanf_s(line, "%d", &duration) != 1)
            {
                printf("Invalid input for duration.\n");
                continue;
            }

            addSong(foundPlaylist, title, artist, duration);
            printf("Song added to playlist '%s' successfully.\n", playlistName);
            break;
        }
        case 4:
        {
            printf("Enter playlist name: ");
            if (!fgets(line, sizeof(line), stdin))
            {
                printf("Error reading input.\n");
                continue;
            }
            line[strcspn(line, "\n")] = 0; 
            Playlist* foundPlaylist = searchPlaylist(table, line);
            if (foundPlaylist)
            {
                displayPlaylist(foundPlaylist);
            }
            else
            {
                printf("Playlist not found.\n");
            }
            break;
        }
        case 5:
        {
            printf("Enter song title: ");
            if (!fgets(line, sizeof(line), stdin))
            {
                printf("Error reading input.\n");
                continue;
            }
            line[strcspn(line, "\n")] = 0;  
            searchPlaylistsBySong(table, line);
            break;
        }
        case 6:

            for (int i = 0; i < HASH_SIZE; i++)
            {
                Playlist* current = table->table[i];
                while (current)
                {
                    Song* song = current->head;
                    while (song)
                    {
                        Song* tempSong = song;
                        song = song->next;
                        free(tempSong);
                    }
                    Playlist* tempPlaylist = current;
                    current = current->next;
                    free(tempPlaylist);
                }
            }
            free(table);
            printf("Exiting program.\n");
            return 0;
        default:
            printf("Invalid choice. Please enter a number between 1 and 6.\n");
        }
    }

    return 0;
}

int isValidPlaylistName(const char* name) 
{
    // Check if the name is not empty and not too long
    return (name != NULL && strlen(name) > 0 && strlen(name) < 100);
}

int isValidSongTitle(const char* title) 
{
    // Check if the title is not empty and not too long
    return (title != NULL && strlen(title) > 0 && strlen(title) < 100);
}

int isValidArtistName(const char* artist) 
{
    // Check if the artist name is not empty and not too long
    return (artist != NULL && strlen(artist) > 0 && strlen(artist) < 100);
}

int isValidDuration(int duration) 
{
    // Check if the duration is a positive number
    return (duration > 0);
}
