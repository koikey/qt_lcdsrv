/****************************************************************************
 LcdView.cpp
****************************************************************************/
#include <QDebug>
#include <QGridLayout>
#include <QByteArray>
#include <QDataStream>
#include <QHostAddress>
#include <QGridLayout>

#include "LcdView.h"

// Class : LcdView Class

LcdView::LcdView( void ) 
{
	bool ret = true;
	qDebug() << "Constructor is called!";

	ret = initLayout();
	if( false == ret ){
		qDebug() << "initLayout is failed !";
		return;
	}
	ret = initUdpSocket();
	if( false == ret ){
		qDebug() << "initBinfile is failed !";
		return;
	}
}

bool LcdView::initLayout(void)
{
	bool ret = true;
	
	qDebug() << "Initialized lcd layout.";

	QGridLayout	*lcdLayout = new QGridLayout( this );
	for( qint32 rowNum = 0; rowNum < 2; rowNum++ ){
		for( qint32 colNum = 0; colNum < 20; colNum++ ){
			m_lcdLabel[rowNum][colNum]	= new QLabel( this );
			m_lcdLabel[rowNum][colNum]->setMinimumWidth(12);
			m_lcdLabel[rowNum][colNum]->setMinimumHeight(12);
			m_lcdLabel[rowNum][colNum]->setFont( QFont( "Monospace" ) );
			lcdLayout->addWidget(
					m_lcdLabel[rowNum][colNum],
					rowNum,		// row
					colNum,		// column
					1,		// height
					1 );		// width
		}
	}

	// Set layout to dialog( e.q LcdView ).
	setWindowTitle( "MS-8300TRX" );
	setLayout( lcdLayout );

	return ret;
}

bool LcdView::initUdpSocket(void)
{
	bool ret = true;

	m_needLength = 0;
	m_rcvData.clear();
	m_udpSocket = new QUdpSocket( this );
	if( NULL == m_udpSocket ){
		qDebug() << "Failed to initialized udp socket!" << endl;
		return (false);
	}
	m_udpSocket->bind( QHostAddress::LocalHost, LCDVIEW_CMD_UDP_PORT );
	connect( m_udpSocket, SIGNAL( readyRead() ), this, SLOT( readPendingDatagrams() ) );

	return ret;
}

void LcdView::readPendingDatagrams(void)
{
	if( NULL == m_udpSocket ){
		qDebug() << "Udp socket is not exist!" << endl;
		return;
	}

	QByteArray	datagram;

	// UDPから受信する
	while( m_udpSocket->hasPendingDatagrams() ){
		datagram.resize( m_udpSocket->pendingDatagramSize() );
		QHostAddress	sender;
		quint16		senderPort;

		m_udpSocket->readDatagram( 
				datagram.data(), 
				datagram.size(),
		                &sender,
				&senderPort);

	}

	quint8		code;		// 制御コード
	quint8		colLen;		// 更新対象の文字列の長さ
	quint8		rowNum = 0;	// 更新対象の行
	quint8		colNum = 0;	// 更新対象の列(更新開始位置)

	code = datagram.at( 0 );
	// 表示指示でない場合は何もせず終了
	if( 0 == (code & LCDVIEW_CMD_BITMASK_PRINT) ){
		qDebug() << "Not support control code." << endl;
		return;
	}

	// 更新対象の行位置を指定
	rowNum = ( 0 == ( code & LCDVIEW_CMD_BITMASK_ROW ) ? 0x00 : 0x01 );
	// 更新開始桁位置を指定
	colNum = ( code & LCDVIEW_CMD_BITMASK_COL );

	// 書き込み長さが表示エリア(幅20文字)までに収まるようにする
	colLen = datagram.at( 1 );
	if( 20 < ( colNum + colLen ) ){
		colLen = 20 - colNum;
	}
	//printf("%d %d %d\n", rowNum, colNum, colLen );

	// 表示エリアを更新
	for( qint32 cnt = 0; cnt < colLen; cnt++ ){
		m_lcdLabel[rowNum][colNum + cnt]->setText( QString( datagram.at( cnt + 2 ) ) );
	}
}
