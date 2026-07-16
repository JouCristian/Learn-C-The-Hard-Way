#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DATA 512
#define MAX_ROWS 100

struct Address
{
    int id;
    int set;
    char name[MAX_DATA];
    char email[MAX_DATA];
};

struct Database
{
    struct Address rows[MAX_ROWS];
};

struct Connection
{
    FILE *file;
    struct Database *db;
};

/*
 * Database_close在die函数中会被提前调用，
 * 所以需要先给出函数声明。
 */
static void Database_close(struct Connection *conn);

/*
 * 处理不依赖errno的普通错误。
 */
static void die(struct Connection *conn, const char *message)
{
    Database_close(conn);
    fprintf(stderr, "ERROR: %s\n", message);
    exit(EXIT_FAILURE);
}

/*
 * 处理由系统库函数产生的错误。
 */
static void die_errno(struct Connection *conn, const char *message)
{
    /*
     * Database_close中的fclose可能改变errno，
     * 所以要先保存真正的错误编号。
     */
    int saved_errno = errno;

    Database_close(conn);

    errno = saved_errno;
    perror(message);

    exit(EXIT_FAILURE);
}

static void Address_print(const struct Address *addr)
{
    printf("%d %s %s\n",
           addr->id,
           addr->name,
           addr->email);
}

static void Database_load(struct Connection *conn)
{
    size_t rc = fread(conn->db,
                      sizeof(*conn->db),
                      1,
                      conn->file);

    if (rc != 1)
    {
        if (ferror(conn->file))
        {
            die_errno(conn, "Failed to load database");
        }

        die(conn, "Database file is incomplete or corrupt");
    }
}

static struct Connection *Database_open(const char *filename,
                                        char mode)
{
    /*
     * calloc申请内存并将其初始化为0。
     */
    struct Connection *conn =
        calloc(1, sizeof(*conn));

    if (conn == NULL)
    {
        die_errno(NULL, "Cannot allocate connection");
    }

    conn->db = calloc(1, sizeof(*conn->db));

    if (conn->db == NULL)
    {
        die_errno(conn, "Cannot allocate database");
    }

    if (mode == 'c')
    {
        /*
         * wb+：以二进制读写模式创建文件，
         * 如果文件已存在则清空原有内容。
         */
        conn->file = fopen(filename, "wb+");
    }
    else
    {
        /*
         * rb+：以二进制读写模式打开已有文件。
         */
        conn->file = fopen(filename, "rb+");
    }

    if (conn->file == NULL)
    {
        die_errno(conn, "Failed to open database file");
    }

    if (mode != 'c')
    {
        Database_load(conn);
    }

    return conn;
}

static void Database_close(struct Connection *conn)
{
    if (conn == NULL)
    {
        return;
    }

    if (conn->file != NULL)
    {
        fclose(conn->file);
    }

    free(conn->db);
    free(conn);
}

static void Database_write(struct Connection *conn)
{
    /*
     * 将文件位置重新移动到开头。
     */
    rewind(conn->file);

    size_t rc = fwrite(conn->db,
                       sizeof(*conn->db),
                       1,
                       conn->file);

    if (rc != 1)
    {
        die_errno(conn, "Failed to write database");
    }

    /*
     * 将C库缓冲区中的数据提交给文件系统。
     */
    if (fflush(conn->file) == EOF)
    {
        die_errno(conn, "Cannot flush database");
    }
}

static void Database_create(struct Connection *conn)
{
    int i = 0;

    for (i = 0; i < MAX_ROWS; i++)
    {
        /*
         * 未明确列出的成员会初始化为0。
         */
        struct Address addr = {
            .id = i,
            .set = 0
        };

        /*
         * 结构体可以直接整体赋值。
         */
        conn->db->rows[i] = addr;
    }
}

/*
 * 安全地将字符串复制到定长字段。
 */
static void copy_field(struct Connection *conn,
                       char *destination,
                       size_t destination_size,
                       const char *source,
                       const char *error_message)
{
    size_t length = strlen(source);

    /*
     * 必须额外预留一个字节存放'\0'。
     */
    if (length >= destination_size)
    {
        die(conn, error_message);
    }

    /*
     * length + 1会把结尾的'\0'一起复制。
     */
    memcpy(destination, source, length + 1);
}

static void Database_set(struct Connection *conn,
                         int id,
                         const char *name,
                         const char *email)
{
    struct Address *addr =
        &conn->db->rows[id];

    if (addr->set)
    {
        die(conn,
            "Record already set; delete it first");
    }

    copy_field(conn,
               addr->name,
               sizeof(addr->name),
               name,
               "Name is too long");

    copy_field(conn,
               addr->email,
               sizeof(addr->email),
               email,
               "Email is too long");

    addr->set = 1;
}

static void Database_find(const struct Connection *conn, const char *name)
{
	int i = 0;
	int found = 0;

	for(i = 0; i< MAX_ROWS; i++){
		const struct Address *current = &conn->db->rows[i];
		if(current->set && strcmp(current->name,name) == 0){
			Address_print(current);
			found = -1;
		}
	}
	if(!found){
		die(NULL, "No matching record!");
	}
}

static void Database_get(struct Connection *conn,
                         int id)
{
    struct Address *addr =
        &conn->db->rows[id];

    if (!addr->set)
    {
        die(conn, "ID is not set");
    }

    Address_print(addr);
}

static void Database_delete(struct Connection *conn,
                            int id)
{
    struct Address addr = {
        .id = id,
        .set = 0
    };

    conn->db->rows[id] = addr;
}

static void Database_list(const struct Connection *conn)
{
    int i = 0;

    for (i = 0; i < MAX_ROWS; i++)
    {
        const struct Address *current =
            &conn->db->rows[i];

        if (current->set)
        {
            Address_print(current);
        }
    }
}

/*
 * 将命令行字符串安全地转换为记录ID。
 */
static int parse_id(const char *text)
{
    char *end = NULL;

    errno = 0;

    long value = strtol(text, &end, 10);

    if (errno != 0)
    {
        die_errno(NULL, "Invalid record ID");
    }

    /*
     * end == text表示一个数字都没有成功转换。
     * *end != '\0'表示数字后面还有非法字符。
     */
    if (end == text || *end != '\0')
    {
        die(NULL,
            "Record ID must be an integer");
    }

    if (value < 0 || value >= MAX_ROWS)
    {
        die(NULL,
            "Record ID is outside the valid range");
    }

    return (int)value;
}

static void validate_arguments(int argc,
                               char *argv[])
{
    if (argc < 3)
    {
        die(NULL,
            "USAGE: ex17 <dbfile> <action> "
            "[action parameters]");
    }

    /*
     * 动作必须恰好是一个字符。
     */
    if (argv[2][0] == '\0' ||
        argv[2][1] != '\0')
    {
        die(NULL,
            "Action must be one character");
    }

    switch (argv[2][0])
    {
        case 'c':
        case 'l':
            if (argc != 3)
            {
                die(NULL,
                    "Create and list take "
                    "no extra parameters");
            }
            break;

        case 'g':
        case 'd':
            if (argc != 4)
            {
                die(NULL,
                    "Get and delete require an ID");
            }
            break;
	
	case 'f':
	    if (argc != 4)
	    {
	    	die(NULL,
		    "Find requires a name");
	    }
	    break;

        case 's':
            if (argc != 6)
            {
                die(NULL,
                    "Set requires ID, name, and email");
            }
            break;

        default:
            die(NULL,
                "Invalid action: use "
                "c=create, g=get, s=set, "
                "d=delete, l=list");
    }
}

int main(int argc, char *argv[])
{
    validate_arguments(argc, argv);

    const char *filename = argv[1];
    char action = argv[2][0];
    int id = 0;

    if (action == 'g' ||
        action == 's' ||
        action == 'd')
    {
        id = parse_id(argv[3]);
    }

    struct Connection *conn =
        Database_open(filename, action);

    switch (action)
    {
        case 'c':
            Database_create(conn);
            Database_write(conn);
            break;

        case 'g':
            Database_get(conn, id);
            break;

        case 's':
            Database_set(conn,
                         id,
                         argv[4],
                         argv[5]);
            Database_write(conn);
            break;

        case 'd':
            Database_delete(conn, id);
            Database_write(conn);
            break;

        case 'l':
            Database_list(conn);
            break;

	case 'f':
	    Database_find(conn,argv[3]);
	    break;
    }

    Database_close(conn);

    return EXIT_SUCCESS;
}
