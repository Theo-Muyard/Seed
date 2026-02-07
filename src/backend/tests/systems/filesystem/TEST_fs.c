#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "seed.h"
#include "core/manager.h"
#include "core/dispatcher.h"
#include "systems/filesystem/system.h"
#include "systems/filesystem/commands.h"

// +===----- Colors -----===+ //

#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define WHITE   "\033[37m"

// +===----- Utils -----===+ //

static void print_section(const char *section)
{
    printf("\n%s=== %s ===%s\n", BLUE, section, WHITE);
}

static void print_success(const char *msg)
{
    printf("%s✓ %s%s\n", GREEN, msg, WHITE);
}

static void print_error(const char *msg)
{
    printf("%s✗ %s%s\n", RED, msg, WHITE);
}

// +===----- Test Cases -----===+ //

static int test_root_open_close(t_Manager *manager)
{
    t_Command       cmd;
    t_CmdOpenRoot   payload;

    print_section("ROOT OPEN / CLOSE");

    payload.path = "/tmp/seed_fs_test";
    cmd.id = CMD_FS_OPEN_ROOT;
    cmd.payload = &payload;

    if (manager_exec(manager, &cmd))
        return (print_error("Failed to open root"), 1);
    print_success("Root opened");

    cmd.id = CMD_FS_CLOSE_ROOT;
    cmd.payload = NULL;

    if (manager_exec(manager, &cmd))
        return (print_error("Failed to close root"), 1);
    print_success("Root closed");

    return (0);
}

static int test_directory_and_file_lifecycle(t_Manager *manager)
{
    t_Command cmd;

    print_section("DIRECTORY & FILE LIFECYCLE");

    // reopen root
    t_CmdOpenRoot open_root = { .path = "/tmp/seed_fs_test" };
    cmd.id = CMD_FS_OPEN_ROOT;
    cmd.payload = &open_root;
    if (manager_exec(manager, &cmd))
        return (print_error("Failed to open root"), 1);

    // create dir: test/
    t_CmdCreateDir mkdir = { .path = "test" };
    cmd.id = CMD_FS_CREATE_DIR;
    cmd.payload = &mkdir;

	int	_code = manager_exec(manager, &cmd);
	printf("%d\n", _code);
    if (manager_exec(manager, &cmd))
        return (print_error("Failed to create directory"), 1);
    print_success("Directory created");

    // create file: test/file.txt
    t_CmdCreateFile mkfile = { .path = "test/file.txt" };
    cmd.id = CMD_FS_CREATE_FILE;
    cmd.payload = &mkfile;

    if (manager_exec(manager, &cmd))
        return (print_error("Failed to create file"), 1);
    print_success("File created");

    // write file
    t_CmdEditDataFile edit = {
        .path = "test/file.txt",
        .data = "hello seed filesystem\n"
    };
    cmd.id = CMD_FS_EDIT_FILE;
    cmd.payload = &edit;

    if (manager_exec(manager, &cmd))
        return (print_error("Failed to edit file"), 1);
    print_success("File edited");

    // read file
    t_CmdReadFile read = { .path = "test/file.txt" };
    cmd.id = CMD_FS_READ_FILE;
    cmd.payload = &read;

    if (manager_exec(manager, &cmd))
        return (print_error("Failed to read file"), 1);
    print_success("File read");

    if (strcmp(read.out_data, "hello seed filesystem\n") != 0)
        return (print_error("File content mismatch"), 1);
    print_success("File content validated");

    // move file
    t_CmdMoveFile mv = {
        .old_path = "test/file.txt",
        .new_path = "test/file2.txt"
    };
    cmd.id = CMD_FS_MOVE_FILE;
    cmd.payload = &mv;

    if (manager_exec(manager, &cmd))
        return (print_error("Failed to move file"), 1);
    print_success("File moved");

    // delete file
    t_CmdDeleteFile rm = { .path = "test/file2.txt" };
    cmd.id = CMD_FS_DELETE_FILE;
    cmd.payload = &rm;

    if (manager_exec(manager, &cmd))
        return (print_error("Failed to delete file"), 1);
    print_success("File deleted");

    // delete dir
    t_CmdDeleteDir rmdir = { .path = "test" };
    cmd.id = CMD_FS_DELETE_DIR;
    cmd.payload = &rmdir;

    if (manager_exec(manager, &cmd))
        return (print_error("Failed to delete directory"), 1);
    print_success("Directory deleted");

    return (0);
}

// +===----- Main -----===+ //

int main(void)
{
    t_Manager *manager;
    int status = 0;

    printf("\n%s╔════════════════════════════════════╗%s\n", BLUE, WHITE);
    printf("%s║      FILESYSTEM TEST SUITE         ║%s\n", BLUE, WHITE);
    printf("%s╚════════════════════════════════════╝%s\n", BLUE, WHITE);

    manager = malloc(sizeof(t_Manager));
    if (!manager)
        return (1);

    if (!dispatcher_init(manager, 32))
        return (print_error("Dispatcher init failed"), 1);

    fs_init(manager);

    status |= test_root_open_close(manager);
	if (status)
		return (1);
    status |= test_directory_and_file_lifecycle(manager);

    dispatcher_clean(manager->dispatcher);
    free(manager);

    if (status == 0)
    {
        printf("\n%s╔════════════════════════════════════╗%s\n", GREEN, WHITE);
        printf("%s║         ALL TESTS PASSED ✓         ║%s\n", GREEN, WHITE);
        printf("%s╚════════════════════════════════════╝%s\n", GREEN, WHITE);
    }
    else
    {
        printf("\n%s╔════════════════════════════════════╗%s\n", RED, WHITE);
        printf("%s║         SOME TESTS FAILED ✗        ║%s\n", RED, WHITE);
        printf("%s╚════════════════════════════════════╝%s\n", RED, WHITE);
    }

    printf("\n");
    return (status);
}
