#include "sierrachart.h"
#include <fstream>
#include <iomanip>
#include <cmath>

SCDLLName("MDTGrep")

SCSFExport scsf_MDTGrep(SCStudyInterfaceRef sc)
{
    // Configuration
    if (sc.SetDefaults)
    {
        sc.GraphName = "MDT Grep (CSV Exporter)";
        sc.StudyDescription = "Exports chart data to CSV (unix_ts_ms, open, high, low, close...)\nAdaptable for tick-level data. Triggered on chart recalculation.";
        
        sc.AutoLoop = 0; // Manual loop to control export logic efficiently
        sc.GraphRegion = 0;
        
        // Input to control export behavior
        sc.Input[0].Name = "Trigger Export on Full Recalc";
        sc.Input[0].SetYesNo(1);

        // Input to control the filename suffix
        sc.Input[1].Name = "Export File Suffix (e.g. _60s, _tick)";
        sc.Input[1].SetString("_tick"); 

        return;
    }

    if (!sc.Input[0].GetYesNo())
        return;

    // Wait until downloading historical data is complete
    if (sc.DownloadingHistoricalData)
        return;

    // Flag to keep track of whether we've exported this recalculation cycle
    int& ExportDone = sc.GetPersistentInt(0);
    
    // Reset our "ExportDone" flag every time a full recalculation is triggered (e.g., Edit -> Refresh on chart)
    if (sc.IsFullRecalculation)
    {
        ExportDone = 0;
    }

    if (ExportDone == 1)
        return; // Already exported this session
        
    // Execute export if we have data
    if (sc.ArraySize == 0) 
        return;

    // Determine the export filename (e.g., ESM4_tick.csv)
    SCString Symbol = sc.Symbol;
    SCString Suffix = sc.Input[1].GetString();
    if (Suffix.IsEmpty())
        Suffix = "_data";

    SCString Filename;
    Filename.Format("%s\\%s%s.csv", sc.DataFilesFolder().GetChars(), Symbol.GetChars(), Suffix.GetChars());

    std::ofstream OutFile(Filename.GetChars(), std::ios::out | std::ios::trunc);
    if (!OutFile.is_open())
    {
        SCString Msg;
        Msg.Format("MDTGrep: Failed to open file for writing: %s", Filename.GetChars());
        sc.AddMessageToLog(Msg, 1);
        return;
    }

    // Write Header: unix_ts_ms, open, high, low, close, volume, ask_vol, bid_vol, num_trades, open_interest
    OutFile << "unix_ts_ms,open,high,low,close,volume,ask_vol,bid_vol,num_trades,open_interest\n";

    // Set standard output precision to accurately represent price/volume 
    OutFile << std::fixed << std::setprecision(5);

    for (int i = 0; i < sc.ArraySize; ++i)
    {
        // Convert the bar's DateTime to a Unix Timestamp in Milliseconds
        // Sierra Chart's SCDateTime represents dates as days since Dec 30, 1899.
        // Unix Epoch (Jan 1, 1970) is 25569 days after Dec 30, 1899.
        // 1 day = 86400000 milliseconds
        double SCDateVal = sc.BaseDateTimeIn[i].GetAsDouble();
        int64_t UnixTS_ms = (int64_t)std::round((SCDateVal - 25569.0) * 86400000.0);
        
        float Open = sc.Open[i];
        float High = sc.High[i];
        float Low = sc.Low[i];
        float Close = sc.Close[i];
        float Volume = sc.Volume[i];
        float AskVol = sc.AskVolume[i];
        float BidVol = sc.BidVolume[i];
        float NumTrades = sc.NumberOfTrades[i];
        float OpenInterest = sc.OpenInterest[i];

        OutFile << UnixTS_ms << ","
                << Open << ","
                << High << ","
                << Low << ","
                << Close << ","
                << Volume << ","
                << AskVol << ","
                << BidVol << ","
                << NumTrades << ","
                << OpenInterest << "\n";
    }

    OutFile.close();
    
    SCString Msg;
    Msg.Format("MDTGrep: Successfully exported %d records to %s", sc.ArraySize, Filename.GetChars());
    sc.AddMessageToLog(Msg, 0);

    // Mark as done so we don't re-export on every tick
    ExportDone = 1;
}
