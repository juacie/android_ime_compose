#ifndef __Py_DATA_UI_H_
#define __Py_DATA_UI_H_

#define MAX_Py1_DATA_UI                10

struct Py1_DATA_UI
{
       struct Py1_DATA_ID
       {
            wchar_t*          WData;
            wchar_t*          OData;
       };
       
       Py1_DATA_ID
       DataUI[MAX_Py1_DATA_UI];
       
       ~Py1_DATA_UI()
       {
            for ( int i =0; i <MAX_Py1_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       Py1_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[4 +1];
            DataUI[ 0].WData[ 0] =0x5427;
            DataUI[ 0].WData[ 1] =0x3a;
            DataUI[ 0].WData[ 2] =0x62;
            DataUI[ 0].WData[ 3] =0x61;
            DataUI[ 0].WData[ 4] =0;

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[4 +1];
            DataUI[ 1].WData[ 0] =0x628a;
            DataUI[ 1].WData[ 1] =0x3a;
            DataUI[ 1].WData[ 2] =0x62;
            DataUI[ 1].WData[ 3] =0x61;
            DataUI[ 1].WData[ 4] =0;

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[4 +1];
            DataUI[ 2].WData[ 0] =0x516b;
            DataUI[ 2].WData[ 1] =0x3a;
            DataUI[ 2].WData[ 2] =0x62;
            DataUI[ 2].WData[ 3] =0x61;
            DataUI[ 2].WData[ 4] =0;

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[4 +1];
            DataUI[ 3].WData[ 0] =0x7238;
            DataUI[ 3].WData[ 1] =0x3a;
            DataUI[ 3].WData[ 2] =0x62;
            DataUI[ 3].WData[ 3] =0x61;
            DataUI[ 3].WData[ 4] =0;

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[4 +1];
            DataUI[ 4].WData[ 0] =0x5df4;
            DataUI[ 4].WData[ 1] =0x3a;
            DataUI[ 4].WData[ 2] =0x62;
            DataUI[ 4].WData[ 3] =0x61;
            DataUI[ 4].WData[ 4] =0;

            //------------------------------------------------------
            // Index 5
            //------------------------------------------------------
            DataUI[ 5].WData =new wchar_t[4 +1];
            DataUI[ 5].WData[ 0] =0x62d4;
            DataUI[ 5].WData[ 1] =0x3a;
            DataUI[ 5].WData[ 2] =0x62;
            DataUI[ 5].WData[ 3] =0x61;
            DataUI[ 5].WData[ 4] =0;

            //------------------------------------------------------
            // Index 6
            //------------------------------------------------------
            DataUI[ 6].WData =new wchar_t[4 +1];
            DataUI[ 6].WData[ 0] =0x7f62;
            DataUI[ 6].WData[ 1] =0x3a;
            DataUI[ 6].WData[ 2] =0x62;
            DataUI[ 6].WData[ 3] =0x61;
            DataUI[ 6].WData[ 4] =0;

            //------------------------------------------------------
            // Index 7
            //------------------------------------------------------
            DataUI[ 7].WData =new wchar_t[4 +1];
            DataUI[ 7].WData[ 0] =0x9738;
            DataUI[ 7].WData[ 1] =0x3a;
            DataUI[ 7].WData[ 2] =0x62;
            DataUI[ 7].WData[ 3] =0x61;
            DataUI[ 7].WData[ 4] =0;

            //------------------------------------------------------
            // Index 8
            //------------------------------------------------------
            DataUI[ 8].WData =new wchar_t[4 +1];
            DataUI[ 8].WData[ 0] =0x575d;
            DataUI[ 8].WData[ 1] =0x3a;
            DataUI[ 8].WData[ 2] =0x62;
            DataUI[ 8].WData[ 3] =0x61;
            DataUI[ 8].WData[ 4] =0;

            //------------------------------------------------------
            // Index 9
            //------------------------------------------------------
            DataUI[ 9].WData =new wchar_t[4 +1];
            DataUI[ 9].WData[ 0] =0x6252;
            DataUI[ 9].WData[ 1] =0x3a;
            DataUI[ 9].WData[ 2] =0x62;
            DataUI[ 9].WData[ 3] =0x61;
            DataUI[ 9].WData[ 4] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
