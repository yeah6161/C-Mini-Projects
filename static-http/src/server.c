#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/stat.h>

#define PORT 8080
#define ROOT "./www"

void send_404(int client) {
    char *msg = "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\nNot Found";
    send(client, msg, strlen(msg), 0);
}

const char *content_type(const char *path) {
    const char *ext = strrchr(path, '.');
    if (!ext) return "application/octet-stream";
    if (strcmp(ext, ".html")==0) return "text/html";
    if (strcmp(ext, ".css")==0) return "text/css";
    if (strcmp(ext, ".js")==0) return "application/javascript";
    if (strcmp(ext, ".png")==0) return "image/png";
    if (strcmp(ext, ".jpg")==0) return "image/jpeg";
    return "application/octet-stream";
}

void handle_client(int client) {
    char buf[4096];
    int r = recv(client, buf, sizeof(buf)-1, 0);
    if (r<=0) { close(client); return; }
    buf[r]=0;
    char method[16], path[1024];
    sscanf(buf, "%15s %1023s", method, path);
    if (strcmp(method, "GET")!=0) { send_404(client); close(client); return; }
    char full[2048];
    if (strcmp(path, "/")==0) strcpy(path, "/index.html");
    snprintf(full, sizeof(full), "%s%s", ROOT, path);
    struct stat st;
    if (stat(full, &st)==-1) { send_404(client); close(client); return; }
    FILE *f = fopen(full,"rb");
    if (!f) { send_404(client); close(client); return; }
    size_t size = st.st_size;
    const char *ctype = content_type(full);
    char header[512];
    snprintf(header, sizeof(header), "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %zu\r\n\r\n", ctype, size);
    send(client, header, strlen(header), 0);
    char buf2[4096];
    size_t read;
    while ((read = fread(buf2,1,sizeof(buf2),f))>0) send(client, buf2, read, 0);
    fclose(f); close(client);
}

int main() {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1; setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    struct sockaddr_in addr;
    addr.sin_family = AF_INET; addr.sin_addr.s_addr = INADDR_ANY; addr.sin_port = htons(PORT);
    bind(sock, (struct sockaddr*)&addr, sizeof(addr));
    listen(sock, 10);
    printf("Listening on port %d, serving %s\n", PORT, ROOT);
    while (1) {
        int client = accept(sock, NULL, NULL);
        if (client>=0) handle_client(client);
    }
    close(sock);
    return 0;
}

