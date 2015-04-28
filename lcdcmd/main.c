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
	unsigned char 	code;				/* 制御コード */
	unsigned char	len;				/* メッセージ長さ */
	char		str[LCD_LINESTR_LEN + 1];	/* 表示メッセージ */
} lcdCmd_t;

int main(
		int 	argc, 
		char 	*argv[]
	)
{
	int 			ret = 0;		/* APIの戻り値 */

	int			sock;			/* ソケットディスクリプタ */
	struct sockaddr_in 	addr;			/* 送信先アドレス情報 */
	int			i;			/* カウンタ */
	lcdCmd_t		lcdCmd;			/* UDP送信コマンド */
	int			dataSize;		/* UDP送信データサイズ */

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

	/* UDPソケット生成 */
	sock = socket( AF_INET, SOCK_DGRAM, 0 );

	/* 送信先アドレス情報生成 */
	addr.sin_family = AF_INET;			/* IPv4 */
	addr.sin_port = htons(5001);			/* ポート:5001 */
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");	/* localhost */
	
	/* LCDコマンド送信 */
	dataSize = sizeof( lcdCmd.code ) + sizeof( lcdCmd.code ) + lcdCmd.len;
	sendto( sock, &lcdCmd, dataSize, 0, ( struct sockaddr * )&addr, sizeof( addr ) );

	close( sock );

	return (ret);
}
