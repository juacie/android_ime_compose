#ifndef __Py3_DATA_UI_H_
#define __Py3_DATA_UI_H_

#define MAX_Py3_DATA_UI                32

struct Py3_DATA_UI
{
       struct Py3_DATA_ID
       {
            wchar_t*          WData;
            wchar_t*          OData;
       };
       
       Py3_DATA_ID
       DataUI[MAX_Py3_DATA_UI];
       
       ~Py3_DATA_UI()
       {
            for ( int i =0; i <MAX_Py3_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       Py3_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[5 +1];
            DataUI[ 0].WData[ 0] =0x505a;
            DataUI[ 0].WData[ 1] =0x3a;
            DataUI[ 0].WData[ 2] =0x7a;
            DataUI[ 0].WData[ 3] =0x75;
            DataUI[ 0].WData[ 4] =0x6f;
            DataUI[ 0].WData[ 5] =0;

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[5 +1];
            DataUI[ 1].WData[ 0] =0x4f5c;
            DataUI[ 1].WData[ 1] =0x3a;
            DataUI[ 1].WData[ 2] =0x7a;
            DataUI[ 1].WData[ 3] =0x75;
            DataUI[ 1].WData[ 4] =0x6f;
            DataUI[ 1].WData[ 5] =0;

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[5 +1];
            DataUI[ 2].WData[ 0] =0x5750;
            DataUI[ 2].WData[ 1] =0x3a;
            DataUI[ 2].WData[ 2] =0x7a;
            DataUI[ 2].WData[ 3] =0x75;
            DataUI[ 2].WData[ 4] =0x6f;
            DataUI[ 2].WData[ 5] =0;

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[5 +1];
            DataUI[ 3].WData[ 0] =0x5de6;
            DataUI[ 3].WData[ 1] =0x3a;
            DataUI[ 3].WData[ 2] =0x7a;
            DataUI[ 3].WData[ 3] =0x75;
            DataUI[ 3].WData[ 4] =0x6f;
            DataUI[ 3].WData[ 5] =0;

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[5 +1];
            DataUI[ 4].WData[ 0] =0x5ea7;
            DataUI[ 4].WData[ 1] =0x3a;
            DataUI[ 4].WData[ 2] =0x7a;
            DataUI[ 4].WData[ 3] =0x75;
            DataUI[ 4].WData[ 4] =0x6f;
            DataUI[ 4].WData[ 5] =0;

            //------------------------------------------------------
            // Index 5
            //------------------------------------------------------
            DataUI[ 5].WData =new wchar_t[5 +1];
            DataUI[ 5].WData[ 0] =0x4f50;
            DataUI[ 5].WData[ 1] =0x3a;
            DataUI[ 5].WData[ 2] =0x7a;
            DataUI[ 5].WData[ 3] =0x75;
            DataUI[ 5].WData[ 4] =0x6f;
            DataUI[ 5].WData[ 5] =0;

            //------------------------------------------------------
            // Index 6
            //------------------------------------------------------
            DataUI[ 6].WData =new wchar_t[5 +1];
            DataUI[ 6].WData[ 0] =0x6628;
            DataUI[ 6].WData[ 1] =0x3a;
            DataUI[ 6].WData[ 2] =0x7a;
            DataUI[ 6].WData[ 3] =0x75;
            DataUI[ 6].WData[ 4] =0x6f;
            DataUI[ 6].WData[ 5] =0;

            //------------------------------------------------------
            // Index 7
            //------------------------------------------------------
            DataUI[ 7].WData =new wchar_t[5 +1];
            DataUI[ 7].WData[ 0] =0x5511;
            DataUI[ 7].WData[ 1] =0x3a;
            DataUI[ 7].WData[ 2] =0x7a;
            DataUI[ 7].WData[ 3] =0x75;
            DataUI[ 7].WData[ 4] =0x6f;
            DataUI[ 7].WData[ 5] =0;

            //------------------------------------------------------
            // Index 8
            //------------------------------------------------------
            DataUI[ 8].WData =new wchar_t[5 +1];
            DataUI[ 8].WData[ 0] =0x7422;
            DataUI[ 8].WData[ 1] =0x3a;
            DataUI[ 8].WData[ 2] =0x7a;
            DataUI[ 8].WData[ 3] =0x75;
            DataUI[ 8].WData[ 4] =0x6f;
            DataUI[ 8].WData[ 5] =0;

            //------------------------------------------------------
            // Index 9
            //------------------------------------------------------
            DataUI[ 9].WData =new wchar_t[5 +1];
            DataUI[ 9].WData[ 0] =0x795a;
            DataUI[ 9].WData[ 1] =0x3a;
            DataUI[ 9].WData[ 2] =0x7a;
            DataUI[ 9].WData[ 3] =0x75;
            DataUI[ 9].WData[ 4] =0x6f;
            DataUI[ 9].WData[ 5] =0;

            //------------------------------------------------------
            // Index 10
            //------------------------------------------------------
            DataUI[10].WData =new wchar_t[5 +1];
            DataUI[10].WData[ 0] =0x562c;
            DataUI[10].WData[ 1] =0x3a;
            DataUI[10].WData[ 2] =0x7a;
            DataUI[10].WData[ 3] =0x75;
            DataUI[10].WData[ 4] =0x6f;
            DataUI[10].WData[ 5] =0;

            //------------------------------------------------------
            // Index 11
            //------------------------------------------------------
            DataUI[11].WData =new wchar_t[5 +1];
            DataUI[11].WData[ 0] =0x51ff;
            DataUI[11].WData[ 1] =0x3a;
            DataUI[11].WData[ 2] =0x7a;
            DataUI[11].WData[ 3] =0x75;
            DataUI[11].WData[ 4] =0x6f;
            DataUI[11].WData[ 5] =0;

            //------------------------------------------------------
            // Index 12
            //------------------------------------------------------
            DataUI[12].WData =new wchar_t[5 +1];
            DataUI[12].WData[ 0] =0x9162;
            DataUI[12].WData[ 1] =0x3a;
            DataUI[12].WData[ 2] =0x7a;
            DataUI[12].WData[ 3] =0x75;
            DataUI[12].WData[ 4] =0x6f;
            DataUI[12].WData[ 5] =0;

            //------------------------------------------------------
            // Index 13
            //------------------------------------------------------
            DataUI[13].WData =new wchar_t[5 +1];
            DataUI[13].WData[ 0] =0x600d;
            DataUI[13].WData[ 1] =0x3a;
            DataUI[13].WData[ 2] =0x7a;
            DataUI[13].WData[ 3] =0x75;
            DataUI[13].WData[ 4] =0x6f;
            DataUI[13].WData[ 5] =0;

            //------------------------------------------------------
            // Index 14
            //------------------------------------------------------
            DataUI[14].WData =new wchar_t[5 +1];
            DataUI[14].WData[ 0] =0x947f;
            DataUI[14].WData[ 1] =0x3a;
            DataUI[14].WData[ 2] =0x7a;
            DataUI[14].WData[ 3] =0x75;
            DataUI[14].WData[ 4] =0x6f;
            DataUI[14].WData[ 5] =0;

            //------------------------------------------------------
            // Index 15
            //------------------------------------------------------
            DataUI[15].WData =new wchar_t[5 +1];
            DataUI[15].WData[ 0] =0x67de;
            DataUI[15].WData[ 1] =0x3a;
            DataUI[15].WData[ 2] =0x7a;
            DataUI[15].WData[ 3] =0x75;
            DataUI[15].WData[ 4] =0x6f;
            DataUI[15].WData[ 5] =0;

            //------------------------------------------------------
            // Index 16
            //------------------------------------------------------
            DataUI[16].WData =new wchar_t[5 +1];
            DataUI[16].WData[ 0] =0x963c;
            DataUI[16].WData[ 1] =0x3a;
            DataUI[16].WData[ 2] =0x7a;
            DataUI[16].WData[ 3] =0x75;
            DataUI[16].WData[ 4] =0x6f;
            DataUI[16].WData[ 5] =0;

            //------------------------------------------------------
            // Index 17
            //------------------------------------------------------
            DataUI[17].WData =new wchar_t[5 +1];
            DataUI[17].WData[ 0] =0x838b;
            DataUI[17].WData[ 1] =0x3a;
            DataUI[17].WData[ 2] =0x7a;
            DataUI[17].WData[ 3] =0x75;
            DataUI[17].WData[ 4] =0x6f;
            DataUI[17].WData[ 5] =0;

            //------------------------------------------------------
            // Index 18
            //------------------------------------------------------
            DataUI[18].WData =new wchar_t[5 +1];
            DataUI[18].WData[ 0] =0x7b2e;
            DataUI[18].WData[ 1] =0x3a;
            DataUI[18].WData[ 2] =0x7a;
            DataUI[18].WData[ 3] =0x75;
            DataUI[18].WData[ 4] =0x6f;
            DataUI[18].WData[ 5] =0;

            //------------------------------------------------------
            // Index 19
            //------------------------------------------------------
            DataUI[19].WData =new wchar_t[5 +1];
            DataUI[19].WData[ 0] =0x80d9;
            DataUI[19].WData[ 1] =0x3a;
            DataUI[19].WData[ 2] =0x7a;
            DataUI[19].WData[ 3] =0x75;
            DataUI[19].WData[ 4] =0x6f;
            DataUI[19].WData[ 5] =0;

            //------------------------------------------------------
            // Index 20
            //------------------------------------------------------
            DataUI[20].WData =new wchar_t[5 +1];
            DataUI[20].WData[ 0] =0x637d;
            DataUI[20].WData[ 1] =0x3a;
            DataUI[20].WData[ 2] =0x7a;
            DataUI[20].WData[ 3] =0x75;
            DataUI[20].WData[ 4] =0x6f;
            DataUI[20].WData[ 5] =0;

            //------------------------------------------------------
            // Index 21
            //------------------------------------------------------
            DataUI[21].WData =new wchar_t[5 +1];
            DataUI[21].WData[ 0] =0x84d9;
            DataUI[21].WData[ 1] =0x3a;
            DataUI[21].WData[ 2] =0x7a;
            DataUI[21].WData[ 3] =0x75;
            DataUI[21].WData[ 4] =0x6f;
            DataUI[21].WData[ 5] =0;

            //------------------------------------------------------
            // Index 22
            //------------------------------------------------------
            DataUI[22].WData =new wchar_t[5 +1];
            DataUI[22].WData[ 0] =0x7a13;
            DataUI[22].WData[ 1] =0x3a;
            DataUI[22].WData[ 2] =0x7a;
            DataUI[22].WData[ 3] =0x75;
            DataUI[22].WData[ 4] =0x6f;
            DataUI[22].WData[ 5] =0;

            //------------------------------------------------------
            // Index 23
            //------------------------------------------------------
            DataUI[23].WData =new wchar_t[5 +1];
            DataUI[23].WData[ 0] =0x923c;
            DataUI[23].WData[ 1] =0x3a;
            DataUI[23].WData[ 2] =0x7a;
            DataUI[23].WData[ 3] =0x75;
            DataUI[23].WData[ 4] =0x6f;
            DataUI[23].WData[ 5] =0;

            //------------------------------------------------------
            // Index 24
            //------------------------------------------------------
            DataUI[24].WData =new wchar_t[5 +1];
            DataUI[24].WData[ 0] =0x4fb3;
            DataUI[24].WData[ 1] =0x3a;
            DataUI[24].WData[ 2] =0x7a;
            DataUI[24].WData[ 3] =0x75;
            DataUI[24].WData[ 4] =0x6f;
            DataUI[24].WData[ 5] =0;

            //------------------------------------------------------
            // Index 25
            //------------------------------------------------------
            DataUI[25].WData =new wchar_t[5 +1];
            DataUI[25].WData[ 0] =0x7b70;
            DataUI[25].WData[ 1] =0x3a;
            DataUI[25].WData[ 2] =0x7a;
            DataUI[25].WData[ 3] =0x75;
            DataUI[25].WData[ 4] =0x6f;
            DataUI[25].WData[ 5] =0;

            //------------------------------------------------------
            // Index 26
            //------------------------------------------------------
            DataUI[26].WData =new wchar_t[5 +1];
            DataUI[26].WData[ 0] =0x8443;
            DataUI[26].WData[ 1] =0x3a;
            DataUI[26].WData[ 2] =0x7a;
            DataUI[26].WData[ 3] =0x75;
            DataUI[26].WData[ 4] =0x6f;
            DataUI[26].WData[ 5] =0;

            //------------------------------------------------------
            // Index 27
            //------------------------------------------------------
            DataUI[27].WData =new wchar_t[5 +1];
            DataUI[27].WData[ 0] =0x64ae;
            DataUI[27].WData[ 1] =0x3a;
            DataUI[27].WData[ 2] =0x7a;
            DataUI[27].WData[ 3] =0x75;
            DataUI[27].WData[ 4] =0x6f;
            DataUI[27].WData[ 5] =0;

            //------------------------------------------------------
            // Index 28
            //------------------------------------------------------
            DataUI[28].WData =new wchar_t[5 +1];
            DataUI[28].WData[ 0] =0x8444;
            DataUI[28].WData[ 1] =0x3a;
            DataUI[28].WData[ 2] =0x7a;
            DataUI[28].WData[ 3] =0x75;
            DataUI[28].WData[ 4] =0x6f;
            DataUI[28].WData[ 5] =0;

            //------------------------------------------------------
            // Index 29
            //------------------------------------------------------
            DataUI[29].WData =new wchar_t[5 +1];
            DataUI[29].WData[ 0] =0x7e53;
            DataUI[29].WData[ 1] =0x3a;
            DataUI[29].WData[ 2] =0x7a;
            DataUI[29].WData[ 3] =0x75;
            DataUI[29].WData[ 4] =0x6f;
            DataUI[29].WData[ 5] =0;

            //------------------------------------------------------
            // Index 30
            //------------------------------------------------------
            DataUI[30].WData =new wchar_t[5 +1];
            DataUI[30].WData[ 0] =0x5c9d;
            DataUI[30].WData[ 1] =0x3a;
            DataUI[30].WData[ 2] =0x7a;
            DataUI[30].WData[ 3] =0x75;
            DataUI[30].WData[ 4] =0x6f;
            DataUI[30].WData[ 5] =0;

            //------------------------------------------------------
            // Index 31
            //------------------------------------------------------
            DataUI[31].WData =new wchar_t[5 +1];
            DataUI[31].WData[ 0] =0x690a;
            DataUI[31].WData[ 1] =0x3a;
            DataUI[31].WData[ 2] =0x7a;
            DataUI[31].WData[ 3] =0x75;
            DataUI[31].WData[ 4] =0x6f;
            DataUI[31].WData[ 5] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
