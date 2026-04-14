#ifndef __Py4_DATA_UI_H_
#define __Py4_DATA_UI_H_

#define MAX_Py4_DATA_UI                    46

struct Py4_DATA_UI
{
       struct Py4_DATA_ID
       {
            wchar_t*        WData;
            wchar_t*        OData;
       };
       
       Py4_DATA_ID
       DataUI[MAX_Py4_DATA_UI];
       
       ~Py4_DATA_UI()
       {
            for ( int i =0; i <MAX_Py4_DATA_UI; i++ )
            {
                  if ( DataUI[i].WData ) 
                       delete DataUI[i].WData;
            }
       }

       Py4_DATA_UI()
       {
            //------------------------------------------------------
            // Index 0
            //------------------------------------------------------
            DataUI[ 0].WData =new wchar_t[5 +1];
            DataUI[ 0].WData[ 0] =0x54b1;
            DataUI[ 0].WData[ 1] =0x3a;
            DataUI[ 0].WData[ 2] =0x7a;
            DataUI[ 0].WData[ 3] =0x61;
            DataUI[ 0].WData[ 4] =0x6e;
            DataUI[ 0].WData[ 5] =0;

            //------------------------------------------------------
            // Index 1
            //------------------------------------------------------
            DataUI[ 1].WData =new wchar_t[5 +1];
            DataUI[ 1].WData[ 0] =0x8d5e;
            DataUI[ 1].WData[ 1] =0x3a;
            DataUI[ 1].WData[ 2] =0x7a;
            DataUI[ 1].WData[ 3] =0x61;
            DataUI[ 1].WData[ 4] =0x6e;
            DataUI[ 1].WData[ 5] =0;

            //------------------------------------------------------
            // Index 2
            //------------------------------------------------------
            DataUI[ 2].WData =new wchar_t[5 +1];
            DataUI[ 2].WData[ 0] =0x6682;
            DataUI[ 2].WData[ 1] =0x3a;
            DataUI[ 2].WData[ 2] =0x7a;
            DataUI[ 2].WData[ 3] =0x61;
            DataUI[ 2].WData[ 4] =0x6e;
            DataUI[ 2].WData[ 5] =0;

            //------------------------------------------------------
            // Index 3
            //------------------------------------------------------
            DataUI[ 3].WData =new wchar_t[5 +1];
            DataUI[ 3].WData[ 0] =0x6512;
            DataUI[ 3].WData[ 1] =0x3a;
            DataUI[ 3].WData[ 2] =0x7a;
            DataUI[ 3].WData[ 3] =0x61;
            DataUI[ 3].WData[ 4] =0x6e;
            DataUI[ 3].WData[ 5] =0;

            //------------------------------------------------------
            // Index 4
            //------------------------------------------------------
            DataUI[ 4].WData =new wchar_t[5 +1];
            DataUI[ 4].WData[ 0] =0x8b9a;
            DataUI[ 4].WData[ 1] =0x3a;
            DataUI[ 4].WData[ 2] =0x7a;
            DataUI[ 4].WData[ 3] =0x61;
            DataUI[ 4].WData[ 4] =0x6e;
            DataUI[ 4].WData[ 5] =0;

            //------------------------------------------------------
            // Index 5
            //------------------------------------------------------
            DataUI[ 5].WData =new wchar_t[5 +1];
            DataUI[ 5].WData[ 0] =0x7c2a;
            DataUI[ 5].WData[ 1] =0x3a;
            DataUI[ 5].WData[ 2] =0x7a;
            DataUI[ 5].WData[ 3] =0x61;
            DataUI[ 5].WData[ 4] =0x6e;
            DataUI[ 5].WData[ 5] =0;

            //------------------------------------------------------
            // Index 6
            //------------------------------------------------------
            DataUI[ 6].WData =new wchar_t[5 +1];
            DataUI[ 6].WData[ 0] =0x74d2;
            DataUI[ 6].WData[ 1] =0x3a;
            DataUI[ 6].WData[ 2] =0x7a;
            DataUI[ 6].WData[ 3] =0x61;
            DataUI[ 6].WData[ 4] =0x6e;
            DataUI[ 6].WData[ 5] =0;

            //------------------------------------------------------
            // Index 7
            //------------------------------------------------------
            DataUI[ 7].WData =new wchar_t[5 +1];
            DataUI[ 7].WData[ 0] =0x661d;
            DataUI[ 7].WData[ 1] =0x3a;
            DataUI[ 7].WData[ 2] =0x7a;
            DataUI[ 7].WData[ 3] =0x61;
            DataUI[ 7].WData[ 4] =0x6e;
            DataUI[ 7].WData[ 5] =0;

            //------------------------------------------------------
            // Index 8
            //------------------------------------------------------
            DataUI[ 8].WData =new wchar_t[5 +1];
            DataUI[ 8].WData[ 0] =0x933e;
            DataUI[ 8].WData[ 1] =0x3a;
            DataUI[ 8].WData[ 2] =0x7a;
            DataUI[ 8].WData[ 3] =0x61;
            DataUI[ 8].WData[ 4] =0x6e;
            DataUI[ 8].WData[ 5] =0;

            //------------------------------------------------------
            // Index 9
            //------------------------------------------------------
            DataUI[ 9].WData =new wchar_t[5 +1];
            DataUI[ 9].WData[ 0] =0x8db1;
            DataUI[ 9].WData[ 1] =0x3a;
            DataUI[ 9].WData[ 2] =0x7a;
            DataUI[ 9].WData[ 3] =0x61;
            DataUI[ 9].WData[ 4] =0x6e;
            DataUI[ 9].WData[ 5] =0;

            //------------------------------------------------------
            // Index 10
            //------------------------------------------------------
            DataUI[10].WData =new wchar_t[5 +1];
            DataUI[10].WData[ 0] =0x507a;
            DataUI[10].WData[ 1] =0x3a;
            DataUI[10].WData[ 2] =0x7a;
            DataUI[10].WData[ 3] =0x61;
            DataUI[10].WData[ 4] =0x6e;
            DataUI[10].WData[ 5] =0;

            //------------------------------------------------------
            // Index 11
            //------------------------------------------------------
            DataUI[11].WData =new wchar_t[5 +1];
            DataUI[11].WData[ 0] =0x8d0a;
            DataUI[11].WData[ 1] =0x3a;
            DataUI[11].WData[ 2] =0x7a;
            DataUI[11].WData[ 3] =0x61;
            DataUI[11].WData[ 4] =0x6e;
            DataUI[11].WData[ 5] =0;

            //------------------------------------------------------
            // Index 12
            //------------------------------------------------------
            DataUI[12].WData =new wchar_t[5 +1];
            DataUI[12].WData[ 0] =0x66ab;
            DataUI[12].WData[ 1] =0x3a;
            DataUI[12].WData[ 2] =0x7a;
            DataUI[12].WData[ 3] =0x61;
            DataUI[12].WData[ 4] =0x6e;
            DataUI[12].WData[ 5] =0;

            //------------------------------------------------------
            // Index 13
            //------------------------------------------------------
            DataUI[13].WData =new wchar_t[5 +1];
            DataUI[13].WData[ 0] =0x74da;
            DataUI[13].WData[ 1] =0x3a;
            DataUI[13].WData[ 2] =0x7a;
            DataUI[13].WData[ 3] =0x61;
            DataUI[13].WData[ 4] =0x6e;
            DataUI[13].WData[ 5] =0;

            //------------------------------------------------------
            // Index 14
            //------------------------------------------------------
            DataUI[14].WData =new wchar_t[5 +1];
            DataUI[14].WData[ 0] =0x5139;
            DataUI[14].WData[ 1] =0x3a;
            DataUI[14].WData[ 2] =0x7a;
            DataUI[14].WData[ 3] =0x61;
            DataUI[14].WData[ 4] =0x6e;
            DataUI[14].WData[ 5] =0;

            //------------------------------------------------------
            // Index 15
            //------------------------------------------------------
            DataUI[15].WData =new wchar_t[5 +1];
            DataUI[15].WData[ 0] =0x62f6;
            DataUI[15].WData[ 1] =0x3a;
            DataUI[15].WData[ 2] =0x7a;
            DataUI[15].WData[ 3] =0x61;
            DataUI[15].WData[ 4] =0x6e;
            DataUI[15].WData[ 5] =0;

            //------------------------------------------------------
            // Index 16
            //------------------------------------------------------
            DataUI[16].WData =new wchar_t[5 +1];
            DataUI[16].WData[ 0] =0x7052;
            DataUI[16].WData[ 1] =0x3a;
            DataUI[16].WData[ 2] =0x7a;
            DataUI[16].WData[ 3] =0x61;
            DataUI[16].WData[ 4] =0x6e;
            DataUI[16].WData[ 5] =0;

            //------------------------------------------------------
            // Index 17
            //------------------------------------------------------
            DataUI[17].WData =new wchar_t[5 +1];
            DataUI[17].WData[ 0] =0x5592;
            DataUI[17].WData[ 1] =0x3a;
            DataUI[17].WData[ 2] =0x7a;
            DataUI[17].WData[ 3] =0x61;
            DataUI[17].WData[ 4] =0x6e;
            DataUI[17].WData[ 5] =0;

            //------------------------------------------------------
            // Index 18
            //------------------------------------------------------
            DataUI[18].WData =new wchar_t[5 +1];
            DataUI[18].WData[ 0] =0x9415;
            DataUI[18].WData[ 1] =0x3a;
            DataUI[18].WData[ 2] =0x7a;
            DataUI[18].WData[ 3] =0x61;
            DataUI[18].WData[ 4] =0x6e;
            DataUI[18].WData[ 5] =0;

            //------------------------------------------------------
            // Index 19
            //------------------------------------------------------
            DataUI[19].WData =new wchar_t[5 +1];
            DataUI[19].WData[ 0] =0x7ccc;
            DataUI[19].WData[ 1] =0x3a;
            DataUI[19].WData[ 2] =0x7a;
            DataUI[19].WData[ 3] =0x61;
            DataUI[19].WData[ 4] =0x6e;
            DataUI[19].WData[ 5] =0;

            //------------------------------------------------------
            // Index 20
            //------------------------------------------------------
            DataUI[20].WData =new wchar_t[5 +1];
            DataUI[20].WData[ 0] =0x8b83;
            DataUI[20].WData[ 1] =0x3a;
            DataUI[20].WData[ 2] =0x7a;
            DataUI[20].WData[ 3] =0x61;
            DataUI[20].WData[ 4] =0x6e;
            DataUI[20].WData[ 5] =0;

            //------------------------------------------------------
            // Index 21
            //------------------------------------------------------
            DataUI[21].WData =new wchar_t[5 +1];
            DataUI[21].WData[ 0] =0x8cdb;
            DataUI[21].WData[ 1] =0x3a;
            DataUI[21].WData[ 2] =0x7a;
            DataUI[21].WData[ 3] =0x61;
            DataUI[21].WData[ 4] =0x6e;
            DataUI[21].WData[ 5] =0;

            //------------------------------------------------------
            // Index 22
            //------------------------------------------------------
            DataUI[22].WData =new wchar_t[5 +1];
            DataUI[22].WData[ 0] =0x93e8;
            DataUI[22].WData[ 1] =0x3a;
            DataUI[22].WData[ 2] =0x7a;
            DataUI[22].WData[ 3] =0x61;
            DataUI[22].WData[ 4] =0x6e;
            DataUI[22].WData[ 5] =0;

            //------------------------------------------------------
            // Index 23
            //------------------------------------------------------
            DataUI[23].WData =new wchar_t[5 +1];
            DataUI[23].WData[ 0] =0x8e54;
            DataUI[23].WData[ 1] =0x3a;
            DataUI[23].WData[ 2] =0x7a;
            DataUI[23].WData[ 3] =0x61;
            DataUI[23].WData[ 4] =0x6e;
            DataUI[23].WData[ 5] =0;

            //------------------------------------------------------
            // Index 24
            //------------------------------------------------------
            DataUI[24].WData =new wchar_t[5 +1];
            DataUI[24].WData[ 0] =0x56cb;
            DataUI[24].WData[ 1] =0x3a;
            DataUI[24].WData[ 2] =0x7a;
            DataUI[24].WData[ 3] =0x61;
            DataUI[24].WData[ 4] =0x6e;
            DataUI[24].WData[ 5] =0;

            //------------------------------------------------------
            // Index 25
            //------------------------------------------------------
            DataUI[25].WData =new wchar_t[5 +1];
            DataUI[25].WData[ 0] =0x5646;
            DataUI[25].WData[ 1] =0x3a;
            DataUI[25].WData[ 2] =0x7a;
            DataUI[25].WData[ 3] =0x61;
            DataUI[25].WData[ 4] =0x6e;
            DataUI[25].WData[ 5] =0;

            //------------------------------------------------------
            // Index 26
            //------------------------------------------------------
            DataUI[26].WData =new wchar_t[5 +1];
            DataUI[26].WData[ 0] =0x81e2;
            DataUI[26].WData[ 1] =0x3a;
            DataUI[26].WData[ 2] =0x7a;
            DataUI[26].WData[ 3] =0x61;
            DataUI[26].WData[ 4] =0x6e;
            DataUI[26].WData[ 5] =0;

            //------------------------------------------------------
            // Index 27
            //------------------------------------------------------
            DataUI[27].WData =new wchar_t[5 +1];
            DataUI[27].WData[ 0] =0x5bc1;
            DataUI[27].WData[ 1] =0x3a;
            DataUI[27].WData[ 2] =0x7a;
            DataUI[27].WData[ 3] =0x61;
            DataUI[27].WData[ 4] =0x6e;
            DataUI[27].WData[ 5] =0;

            //------------------------------------------------------
            // Index 28
            //------------------------------------------------------
            DataUI[28].WData =new wchar_t[5 +1];
            DataUI[28].WData[ 0] =0x79b6;
            DataUI[28].WData[ 1] =0x3a;
            DataUI[28].WData[ 2] =0x7a;
            DataUI[28].WData[ 3] =0x61;
            DataUI[28].WData[ 4] =0x6e;
            DataUI[28].WData[ 5] =0;

            //------------------------------------------------------
            // Index 29
            //------------------------------------------------------
            DataUI[29].WData =new wchar_t[5 +1];
            DataUI[29].WData[ 0] =0x9961;
            DataUI[29].WData[ 1] =0x3a;
            DataUI[29].WData[ 2] =0x7a;
            DataUI[29].WData[ 3] =0x61;
            DataUI[29].WData[ 4] =0x6e;
            DataUI[29].WData[ 5] =0;

            //------------------------------------------------------
            // Index 30
            //------------------------------------------------------
            DataUI[30].WData =new wchar_t[5 +1];
            DataUI[30].WData[ 0] =0x9147;
            DataUI[30].WData[ 1] =0x3a;
            DataUI[30].WData[ 2] =0x7a;
            DataUI[30].WData[ 3] =0x61;
            DataUI[30].WData[ 4] =0x6e;
            DataUI[30].WData[ 5] =0;

            //------------------------------------------------------
            // Index 31
            //------------------------------------------------------
            DataUI[31].WData =new wchar_t[6 +1];
            DataUI[31].WData[ 0] =0x810f;
            DataUI[31].WData[ 1] =0x3a;
            DataUI[31].WData[ 2] =0x7a;
            DataUI[31].WData[ 3] =0x61;
            DataUI[31].WData[ 4] =0x6e;
            DataUI[31].WData[ 5] =0x67;
            DataUI[31].WData[ 6] =0;

            //------------------------------------------------------
            // Index 32
            //------------------------------------------------------
            DataUI[32].WData =new wchar_t[6 +1];
            DataUI[32].WData[ 0] =0x85cf;
            DataUI[32].WData[ 1] =0x3a;
            DataUI[32].WData[ 2] =0x7a;
            DataUI[32].WData[ 3] =0x61;
            DataUI[32].WData[ 4] =0x6e;
            DataUI[32].WData[ 5] =0x67;
            DataUI[32].WData[ 6] =0;

            //------------------------------------------------------
            // Index 33
            //------------------------------------------------------
            DataUI[33].WData =new wchar_t[6 +1];
            DataUI[33].WData[ 0] =0x846c;
            DataUI[33].WData[ 1] =0x3a;
            DataUI[33].WData[ 2] =0x7a;
            DataUI[33].WData[ 3] =0x61;
            DataUI[33].WData[ 4] =0x6e;
            DataUI[33].WData[ 5] =0x67;
            DataUI[33].WData[ 6] =0;

            //------------------------------------------------------
            // Index 34
            //------------------------------------------------------
            DataUI[34].WData =new wchar_t[6 +1];
            DataUI[34].WData[ 0] =0x81e7;
            DataUI[34].WData[ 1] =0x3a;
            DataUI[34].WData[ 2] =0x7a;
            DataUI[34].WData[ 3] =0x61;
            DataUI[34].WData[ 4] =0x6e;
            DataUI[34].WData[ 5] =0x67;
            DataUI[34].WData[ 6] =0;

            //------------------------------------------------------
            // Index 35
            //------------------------------------------------------
            DataUI[35].WData =new wchar_t[6 +1];
            DataUI[35].WData[ 0] =0x8d43;
            DataUI[35].WData[ 1] =0x3a;
            DataUI[35].WData[ 2] =0x7a;
            DataUI[35].WData[ 3] =0x61;
            DataUI[35].WData[ 4] =0x6e;
            DataUI[35].WData[ 5] =0x67;
            DataUI[35].WData[ 6] =0;

            //------------------------------------------------------
            // Index 36
            //------------------------------------------------------
            DataUI[36].WData =new wchar_t[6 +1];
            DataUI[36].WData[ 0] =0x9ad2;
            DataUI[36].WData[ 1] =0x3a;
            DataUI[36].WData[ 2] =0x7a;
            DataUI[36].WData[ 3] =0x61;
            DataUI[36].WData[ 4] =0x6e;
            DataUI[36].WData[ 5] =0x67;
            DataUI[36].WData[ 6] =0;

            //------------------------------------------------------
            // Index 37
            //------------------------------------------------------
            DataUI[37].WData =new wchar_t[6 +1];
            DataUI[37].WData[ 0] =0x8535;
            DataUI[37].WData[ 1] =0x3a;
            DataUI[37].WData[ 2] =0x7a;
            DataUI[37].WData[ 3] =0x61;
            DataUI[37].WData[ 4] =0x6e;
            DataUI[37].WData[ 5] =0x67;
            DataUI[37].WData[ 6] =0;

            //------------------------------------------------------
            // Index 38
            //------------------------------------------------------
            DataUI[38].WData =new wchar_t[6 +1];
            DataUI[38].WData[ 0] =0x9a75;
            DataUI[38].WData[ 1] =0x3a;
            DataUI[38].WData[ 2] =0x7a;
            DataUI[38].WData[ 3] =0x61;
            DataUI[38].WData[ 4] =0x6e;
            DataUI[38].WData[ 5] =0x67;
            DataUI[38].WData[ 6] =0;

            //------------------------------------------------------
            // Index 39
            //------------------------------------------------------
            DataUI[39].WData =new wchar_t[6 +1];
            DataUI[39].WData[ 0] =0x5958;
            DataUI[39].WData[ 1] =0x3a;
            DataUI[39].WData[ 2] =0x7a;
            DataUI[39].WData[ 3] =0x61;
            DataUI[39].WData[ 4] =0x6e;
            DataUI[39].WData[ 5] =0x67;
            DataUI[39].WData[ 6] =0;

            //------------------------------------------------------
            // Index 40
            //------------------------------------------------------
            DataUI[40].WData =new wchar_t[6 +1];
            DataUI[40].WData[ 0] =0x7242;
            DataUI[40].WData[ 1] =0x3a;
            DataUI[40].WData[ 2] =0x7a;
            DataUI[40].WData[ 3] =0x61;
            DataUI[40].WData[ 4] =0x6e;
            DataUI[40].WData[ 5] =0x67;
            DataUI[40].WData[ 6] =0;

            //------------------------------------------------------
            // Index 41
            //------------------------------------------------------
            DataUI[41].WData =new wchar_t[6 +1];
            DataUI[41].WData[ 0] =0x81df;
            DataUI[41].WData[ 1] =0x3a;
            DataUI[41].WData[ 2] =0x7a;
            DataUI[41].WData[ 3] =0x61;
            DataUI[41].WData[ 4] =0x6e;
            DataUI[41].WData[ 5] =0x67;
            DataUI[41].WData[ 6] =0;

            //------------------------------------------------------
            // Index 42
            //------------------------------------------------------
            DataUI[42].WData =new wchar_t[6 +1];
            DataUI[42].WData[ 0] =0x81d3;
            DataUI[42].WData[ 1] =0x3a;
            DataUI[42].WData[ 2] =0x7a;
            DataUI[42].WData[ 3] =0x61;
            DataUI[42].WData[ 4] =0x6e;
            DataUI[42].WData[ 5] =0x67;
            DataUI[42].WData[ 6] =0;

            //------------------------------------------------------
            // Index 43
            //------------------------------------------------------
            DataUI[43].WData =new wchar_t[6 +1];
            DataUI[43].WData[ 0] =0x8ccd;
            DataUI[43].WData[ 1] =0x3a;
            DataUI[43].WData[ 2] =0x7a;
            DataUI[43].WData[ 3] =0x61;
            DataUI[43].WData[ 4] =0x6e;
            DataUI[43].WData[ 5] =0x67;
            DataUI[43].WData[ 6] =0;

            //------------------------------------------------------
            // Index 44
            //------------------------------------------------------
            DataUI[44].WData =new wchar_t[6 +1];
            DataUI[44].WData[ 0] =0x8d13;
            DataUI[44].WData[ 1] =0x3a;
            DataUI[44].WData[ 2] =0x7a;
            DataUI[44].WData[ 3] =0x61;
            DataUI[44].WData[ 4] =0x6e;
            DataUI[44].WData[ 5] =0x67;
            DataUI[44].WData[ 6] =0;

            //------------------------------------------------------
            // Index 45
            //------------------------------------------------------
            DataUI[45].WData =new wchar_t[6 +1];
            DataUI[45].WData[ 0] =0x5f09;
            DataUI[45].WData[ 1] =0x3a;
            DataUI[45].WData[ 2] =0x7a;
            DataUI[45].WData[ 3] =0x61;
            DataUI[45].WData[ 4] =0x6e;
            DataUI[45].WData[ 5] =0x67;
            DataUI[45].WData[ 6] =0;

       };

       wchar_t* GetWDataSz(long Index)
       {
            return DataUI[Index].WData;
       }
};

#endif
