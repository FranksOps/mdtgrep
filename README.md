# MDTGrep (Market Data Tick Grep)

MDTGrep is an Advanced Custom Study Interface and Language (ACSIL) DLL for Sierra Chart.

It enables lightning-fast, highly accurate data extraction of your Sierra Chart historical/live datasets into easily parsable flat CSV format. Ideal for strategy backtesting on minute-by-minute or exact 1-tick granularity.

## Core Capabilities

*   **Chart Native Extraction**: Since MDTGrep runs directly on the chart, it automatically inherits everything you are currently looking at on the screen. This means any back-adjustments, continuous contract rollovers, or custom session times you have applied to your Sierra Chart are natively built into the exported data.
*   **Customizable Timeframes**: Bar periodicity natively dictates what exports. Whether you want an export of exactly 60 seconds (1 minute), or highly granular 1 Number of Trades (Tick) data, MDTGrep adapts perfectly to your `Chart Settings -> Bar Period`.
*   **Precise Unix Epochs**: By extracting internal C++ dates into exact UNIX Epoch floating point timestamps (`unix_ts_ms`), timestamps perfectly map to machine-learning models, external analytics suites (Pandas, Polars), or internal database engines.

## Installation

1.  Copy `MDTGrep.cpp` to your Sierra Chart source folder (usually `C:\SierraChart\ACS_Source\`).
2.  Open Sierra Chart.
3.  Go to `Analysis` -> `Build Custom Studies DLL`.
4.  Check `MDTGrep.cpp`.
5.  Click **Build Remote** (or **Build Local** if you have a local compiler setup).

## Getting High-Speed Tick Data

To properly extract true Level-1 Tick Data (Price and Volume mapping exactly how it crossed the tape):
1.  Open the desired Symbol (e.g., `ESM4`).
2.  Hit `F5` to open **Chart Settings**.
3.  Change `Graph Draw Type` to `Candlestick` or `Bar`.
4.  Navigate to the `Bar Period` menu.
5.  Change to `Number of Trades Per Bar`.
6.  Set the value to **1**.
7.  Add `MDTGrep` from `Analysis` -> `Studies`.

## Configuration Options

When you add `MDTGrep` via the `Studies` menu, you have two primary inputs to control the logic without having to rebuild the DLL:

*   **Trigger Export on Full Recalc**: Set to `Yes` by default. This makes sure that data is explicitly written to your hard drive every time you hit `Edit` -> `Recalculate` so that you don't stall your trading view looping the logic every single bid/ask tick. 
*   **Export Mode**: You can select either `Time (OHLC + Vol)` or `Tick (Price + Vol + Delta)`. Depending on your choice, the study automatically applies a `_time.csv` or `_tick.csv` suffix to the file output.

## Generated Columns 

MDTGrep creates the file inside your `\Data\` directory (`C:\SierraChart\Data\`). The exported columns depend on the selected **Export Mode**:

### Time Mode (`_time.csv`)
*   `unix_ts_ms` (standard millisecond Unix timestamp mapped statically to the chart time)
*   `open` 
*   `high`
*   `low`
*   `close`
*   `volume`
*   `ask_vol`
*   `bid_vol`
*   `num_trades`
*   `open_interest`

### Tick Mode (`_tick.csv`)
*   `unix_ts_ms`
*   `price` (the actual traded price for that tick)
*   `volume`
*   `ask_vol`
*   `bid_vol`
*   `delta` (Ask Volume - Bid Volume)
*   `num_trades`

## Example Output

**Time Mode (`_time.csv`)**
```csv
unix_ts_ms,open,high,low,close,volume,ask_vol,bid_vol,num_trades,open_interest
1716301800000,5321.00,5321.75,5321.00,5321.50,154,82,72,98,124500
1716301860000,5321.50,5322.25,5321.25,5321.75,210,115,95,145,124530
```

**Tick Mode (`_tick.csv`)**
```csv
unix_ts_ms,price,volume,ask_vol,bid_vol,delta,num_trades
1716301802150,5321.25,2,2,0,2,1
1716301802300,5321.00,5,0,5,-5,2
```
