/*
	Simple http server that returns an HTML file
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
 
#define BUFLEN	1024	//Max length of buffer
#define PORT	8000	//The port on which to listen for incoming data

char http_ok[] = "HTTP/1.0 200 OK\r\nContent-type: text/html\r\nServer: Test\r\n\r\n";
char http_error[] = "HTTP/1.0 400 Bad Request\r\nContent-type: text/html\r\nServer: Test\r\n\r\n";

// Read the HTML file into a buffer
char *read_file(const char *filename)
{
	FILE *file = fopen(filename, "rb");
	if (!file) {
		perror("fopen");
		return NULL;
	}

	// Get the file size
	fseek(file, 0, SEEK_END);
	long size = ftell(file);
	rewind(file);

	// Allocate memory to store file content
	char *content = (char *)malloc(size + 1);
	if (!content) {
		perror("malloc");
		fclose(file);
		return NULL;
	}

	// Read file content into buffer
	fread(content, 1, size, file);
	content[size] = '\0'; // Null-terminate the string

	fclose(file);
	return content;
}

void update_html()
{
	system("./monitor.sh");
}

void die(char *s)
{
	perror(s);
	exit(1);
}
 
int main(void)
{
	struct sockaddr_in si_me, si_other;

	int s, i, slen = sizeof(si_other) , recv_len, conn;
	char buf[BUFLEN];
     
	/* create a TCP socket */
	if ((s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
		die("socket");
    
	/* zero out the structure */
	memset((char *) &si_me, 0, sizeof(si_me));
     
	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);
     
	/* bind socket to port */
	if (bind(s, (struct sockaddr*)&si_me, sizeof(si_me)) == -1)
		die("bind");
	
	/* allow 10 requests to queue up */ 
	if (listen(s, 10) == -1)
		die("listen");
     
	/* keep listening for data */
	while (1) {
		memset(buf, 0, sizeof(buf));
		printf("Waiting a connection...");
		fflush(stdout);
		
		conn = accept(s, (struct sockaddr *) &si_other, &slen);
		if (conn < 0)
			die("accept");

		printf("Client connected: %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));

		/* try to receive some data, this is a blocking call */
		recv_len = read(conn, buf, BUFLEN);
		if (recv_len < 0)
			die("read");

		/* print details of the client/peer and the data received */
		printf("Data: %s\n" , buf);

		if (strstr(buf, "GET")) {
			// updates the html file to send to the requestor
			update_html();
			char *html_content = read_file("index.html");

			if (html_content) {
				/* Send HTTP OK response */
				if (write(conn, http_ok, strlen(http_ok), 0) < 0)
					die("write");

				/* Send HTML content */
				if (write(conn, html_content, strlen(html_content), 0) < 0)
					die("write");

				/* Free allocated memory */
				free(html_content);
			} else {
				/* Send HTTP error response */
				if (write(conn, http_error, strlen(http_error), 0) < 0)
					die("write");
			}
		}
		/* close the connection */
		close(conn);
	}
	close(s);
	
	return 0;
}