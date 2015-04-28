/****************************************************************************
****************************************************************************/
#ifndef LCD_VIEW_HEADER
#define LCD_VIEW_HEADER

#include <QDialog>
#include <QUdpSocket>
#include <QTcpServer>
#include <QTcpSocket>
#include <QLabel>
#include <QDebug>

#ifdef __cplusplus
extern "C"{
#endif /* __cplusplus */
#ifdef __cplusplus
}
#endif /* __cplusplus */

#define LCDVIEW_CMD_BITMASK_PRINT	(0x80)
#define LCDVIEW_CMD_BITMASK_ROW		(0x40)
#define LCDVIEW_CMD_BITMASK_COL		(0x3F)

#define LCDVIEW_CMD_UDP_PORT	(5001)


// Class : LcdView Class
// Input lcd view.
class LcdView : public QDialog
{
	Q_OBJECT

public:
	LcdView( void );
	
public slots:
	// Udp data recieved slot.
	void readPendingDatagrams( void );
	void acceptConnection( void );
	void recievedLcdCmd( void );
	void displayError( QAbstractSocket::SocketError socketError );
	void refreshLcd( void );

signals:

private:
	QLabel		*m_lcdLabel[2][20];	// LCD•\Ž¦ƒ‰ƒxƒ‹
	QUdpSocket	*m_udpSocket;
	QByteArray	m_rcvData;
	qint64		m_needLength;
	QTcpServer	*m_TcpServer;
	QTcpSocket	*m_TcpSocket;
	
	// Initalized keyview layout.
	bool initLayout( void );
	bool initUdpSocket( void );
	bool initTcpSocket( void );

};

#endif /* LCD_VIEW_HEADER */
