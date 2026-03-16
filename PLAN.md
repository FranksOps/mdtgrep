## REQUIREMENT

https://www.sierrachart.com/index.php?page=doc/AdvancedCustomStudyInterfaceAndLanguage.php

https://github.com/power-shift/sierrachart-acsil-docs

ACSIL study, add to chart, uses chart settings (bar period, range of time, or specific contract name) to download tick data for future analysis, strategy backtesting, algo-bot training.
 
(Time) Back-adjusted - Best for price-based strategies — eliminates roll gaps, levels are continuous
 
    Chart Settings → Bar Period
    1 minute / 60 secondS

    unix_ts, open, high, low, close, volume, ask_vol, bid_vol, num_trades, open_interest

(Tick)No adjustment- Best for volume/delta/profile work — actual traded prices preserved

    Chart Settings → Bar Period
    Number ofTrades Per Bar / 1

    unix_ts_ms, price, volume, ask_vol, bid_vol, delta, num_trades

## FUTURE ENHANCEMENTS

* **Parquet Export**: Eventually transition from CSV to Apache Parquet format. Given that a few years of tick data can amount to 30+ GB, Parquet's columnar storage and efficient compression will significantly reduce file sizes and improve read performance for subsequent analysis and backtesting.
