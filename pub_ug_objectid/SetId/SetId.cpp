/*****************************************************************************
**
** SetId.cpp
**
** Description:
**     Contains Unigraphics entry points for the application.
**
*****************************************************************************/

/* Include files */
#if ! defined ( __hp9000s800 ) && ! defined ( __sgi ) && ! defined ( __sun )
#   include <strstream>
#   include <iostream>
    using std::ostrstream;
    using std::endl;    
    using std::ends;
    using std::cerr;
#else
#   include <strstream.h>
#   include <iostream.h>
#endif
#include <uf.h>
#include <uf_ui.h>
#include <uf_exit.h>

#include <NXOpen/Session.hxx>
#include <NXopen/UI.hxx>
#include <NXOpen/Selection.hxx>
#include <NXOpen/NXObject.hxx>
#include <NXOpen/NXMessageBox.hxx>
#include <NXOpen/ListingWindow.hxx>
#include <NXOpen/PartCollection.hxx>
#include <NXOpen/NXException.hxx>

using namespace NXOpen;

NXObject* GetObjectFormid(int id)
{
	char str[133];
	sprintf(str,"HANDLE R-%d",id);
//	uc1601(str,1);
	NXObject* obj = dynamic_cast<NXObject*>(Session::GetSession()->Parts()->Work()->FindObject(str));

	return obj;
}

#define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))

static int report_error( char *file, int line, char *call, int irc)
{
    if (irc)
    {
        char err[133],
             msg[133];

        sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",
            irc, line, file);
        UF_get_fail_message(irc, err);

        UF_print_syslog(msg, FALSE);
        UF_print_syslog(err, FALSE);
        UF_print_syslog("\n", FALSE);
        UF_print_syslog(call, FALSE);
        UF_print_syslog(";\n", FALSE);

        if (!UF_UI_open_listing_window())
        {
            UF_UI_write_listing_window(msg);
            UF_UI_write_listing_window(err);
            UF_UI_write_listing_window("\n");
            UF_UI_write_listing_window(call);
            UF_UI_write_listing_window(";\n");
        }
    }

    return(irc);
}


/*****************************************************************************
**  Activation Methods
*****************************************************************************/
/*  Explicit Activation
**      This entry point is used to activate the application explicitly, as in
**      "File->Execute UG/Open->User Function..." */
extern DllExport void ufusr( char *parm, int *returnCode, int rlen )
{
    /* Initialize the API environment */
    if( UF_CALL(UF_initialize()) ) 
    {
        /* Failed to initialize */
        return;
    }

	try
	{
		char input[133];
		memset(input,0,133*sizeof(char));
		int length;
		uc1600("Input id",input,&length);

		int id=0;
		sscanf(input,"%d",&id);

		NXObject* obj = GetObjectFormid(id);

		DisplayableObject* dispObject = dynamic_cast<DisplayableObject*>(obj);
		if(dispObject!=NULL)
			dispObject->Highlight();
	}
	catch(NXException* ex)
	{
		uc1601(const_cast<char*>(ex->Message()),1);
	}
	catch (std::exception* e)
	{
		uc1601(const_cast<char*>(e->what()),1);
	}



    /* Terminate the API environment */
    UF_CALL(UF_terminate());
}

/*****************************************************************************
**  Utilities
*****************************************************************************/

/* Unload Handler
**     This function specifies when to unload your application from Unigraphics.
**     If your application registers a callback (from a MenuScript item or a
**     User Defined Object for example), this function MUST return
**     "UF_UNLOAD_UG_TERMINATE". */
extern int ufusr_ask_unload( void )
{
    return( UF_UNLOAD_IMMEDIATELY  );
}

