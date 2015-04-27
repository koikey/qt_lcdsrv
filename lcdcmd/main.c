#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#define		CHRtoHEX(p)		((( p > 0x40 ) ? ( p - 7 ) : p ) & 0x0f )
#define		STRtoHEX(p)		(( CHRtoHEX( p[0] ) << 4 ) | CHRtoHEX( p[1] ))

int main(
		int 	argc, 
		char 	*argv[]
	)
{
	int 			ret = 0;		/* API�̖߂�l */

	char 			cmd[24] = { 0x00 };	/* �R�}���h */
	int			sock;			/* �\�P�b�g�f�B�X�N���v�^ */
	struct sockaddr_in 	addr;			/* ���M��A�h���X��� */
	int			i;			/* �J�E���^ */

	if( argc < 3 ){
		printf( "\x1b[31mUnder Count ... '%s' '%s'\x1b[0m\n", argc > 0 ? argv[0] : "NULL", argc > 1 ? argv[1] : "NULL" );
		exit( 0 );
	}
	
	printf( "\x1b[36m Line %c .... ", argv[1][0] == '8' ? '1' : 
				  argv[1][0] == '9' ? '1' : 
				  argv[1][0] == 'C' ? '2' : 
				  argv[1][0] == 'D' ? '2' : 'X' ) ;
	for( i = 2 ; i < argc ; i++ ){
		cmd[i-2] = STRtoHEX( argv[i] );
		cmd[i-1] = 0x00;
		if( cmd[i-2] == 0x08 )		cmd[i-2] = '^' ;
		if( cmd[i-2] == 0x09 )		cmd[i-2] = '_' ;
		if( cmd[i-2] == 0x0a )		cmd[i-2] = '*' ;
		if( cmd[i-2] == 0x0b )		cmd[i-2] = '+' ;
		if( cmd[i-2] == 0x7e )		cmd[i-2] = '>' ;
		if( cmd[i-2] == 0x7f )		cmd[i-2] = '<' ;
	}
	printf( "\'%s\'\x1b[0m\n", cmd );

	/* UDP�\�P�b�g���� */
	sock = socket( AF_INET, SOCK_DGRAM, 0 );

	/* ���M��A�h���X��񐶐� */
	addr.sin_family = AF_INET;			/* IPv4 */
	addr.sin_port = htons(5001);			/* �|�[�g:5001 */
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");	/* localhost */

	sendto(sock, cmd, sizeof(cmd), 0, (struct sockaddr *)&addr, sizeof(addr));

	close(sock);


	return (ret);
}
