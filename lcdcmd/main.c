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
#define		LCD_LINESTR_LEN		(20U)

#define		LCD_LINE_1		(1)
#define		LCD_LINE_2		(2)
#define		LCD_LINE_INVALID	(0)

/* Send command data type */
typedef struct lcdCmd_tag {
	unsigned char 	code;				/* ����R�[�h */
	unsigned char	len;				/* ���b�Z�[�W���� */
	char		str[LCD_LINESTR_LEN + 1];	/* �\�����b�Z�[�W */
} lcdCmd_t;

int main(
		int 	argc, 
		char 	*argv[]
	)
{
	int 			ret = 0;		/* API�̖߂�l */

	int			sock;			/* �\�P�b�g�f�B�X�N���v�^ */
	struct sockaddr_in 	addr;			/* ���M��A�h���X��� */
	int			i;			/* �J�E���^ */
	lcdCmd_t		lcdCmd;			/* UDP���M�R�}���h */
	int			dataSize;		/* UDP���M�f�[�^�T�C�Y */

	/* Initialized send command data. */
	lcdCmd.code = LCD_LINE_INVALID;
	memset( lcdCmd.str, 0, sizeof(lcdCmd.str) );

	/* Arg check */
	if( ( argc < 3 ) || ( argc > 1 + sizeof(lcdCmd) ) ){
		printf( "\x1b[31mUnder Count ... '%s' '%s'\x1b[0m\n", argc > 0 ? argv[0] : "NULL", argc > 1 ? argv[1] : "NULL" );
		exit( 0 );
	}
	
	/* Set control code */
#if 0
	lcdCmd.code = ( ( argv[1][0] == '8' ) ? LCD_LINE_1 : 
			( argv[1][0] == '9' ) ? LCD_LINE_1 : 
			( argv[1][0] == 'C' ) ? LCD_LINE_2 : 
			( argv[1][0] == 'D' ) ? LCD_LINE_2 : LCD_LINE_INVALID );
	
	printf( "\x1b[36m Line %d .... ", lcdCmd.code ) ;
#endif
	lcdCmd.code = (unsigned char)STRtoHEX( argv[1] );
	lcdCmd.len = argc - 2;
	for( i = 2 ; i < argc ; i++ ){
		lcdCmd.str[i-2] = STRtoHEX( argv[i] );
		lcdCmd.str[i-1] = 0x00;
		if( lcdCmd.str[i-2] == 0x08 )		lcdCmd.str[i-2] = '^' ;
		if( lcdCmd.str[i-2] == 0x09 )		lcdCmd.str[i-2] = '_' ;
		if( lcdCmd.str[i-2] == 0x0a )		lcdCmd.str[i-2] = '*' ;
		if( lcdCmd.str[i-2] == 0x0b )		lcdCmd.str[i-2] = '+' ;
		if( lcdCmd.str[i-2] == 0x7e )		lcdCmd.str[i-2] = '>' ;
		if( lcdCmd.str[i-2] == 0x7f )		lcdCmd.str[i-2] = '<' ;
	}
	printf( "\'%s\'\x1b[0m\n", lcdCmd.str );

	/* UDP�\�P�b�g���� */
	sock = socket( AF_INET, SOCK_DGRAM, 0 );

	/* ���M��A�h���X��񐶐� */
	addr.sin_family = AF_INET;			/* IPv4 */
	addr.sin_port = htons(5001);			/* �|�[�g:5001 */
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");	/* localhost */
	
	/* LCD�R�}���h���M */
	dataSize = sizeof( lcdCmd.code ) + sizeof( lcdCmd.code ) + lcdCmd.len;
	sendto( sock, &lcdCmd, dataSize, 0, ( struct sockaddr * )&addr, sizeof( addr ) );

	close( sock );

	return (ret);
}
