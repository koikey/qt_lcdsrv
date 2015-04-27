/****************************************************************************
****************************************************************************/

#include <QApplication>
#include <QDebug>

#include "LcdView.h"

/****************************************************************************
 * Key input apps main
 ***************************************************************************/
int main( 
		int	argc, 	/* Num of args */
		char 	**argv 	/* List of args */
	)
{
	QApplication app( argc, argv );

	LcdView lcdView;
	lcdView.show();

	return app.exec();
}
