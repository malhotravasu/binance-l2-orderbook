"""
Binance Spot WebSocket market data ingestion.

Streams live cryptocurrency market data (trades, tickers, depth updates)
from Binance and persists to disk. Select 10 actively traded symbols by
default (top market cap: BTC, ETH, XRP, BNB, SOL, TRX, DOGE, ADA, BCH, XLM).
"""

import asyncio
import json
import logging

import websockets

# Configuration
DEFAULT_SYMBOLS = [
    "btcusdt",
    "ethusdt",
    "xrpusdt",
    "bnbusdt",
    "solusdt",
    "trxusdt",
    "dogeusdt",
    "adausdt",
    "bchusdt",
    "xlmusdt",
]
BINANCE_WS_URL = "wss://stream.binance.com:9443/ws"

# Configure logging
logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s - %(levelname)s - %(message)s",
)
logger = logging.getLogger(__name__)


async def stream_market_data(
    symbols: list[str] = DEFAULT_SYMBOLS,
    stream_type: str = "depth",
) -> None:
    """
    Stream market data from Binance WebSocket.

    Args:
        symbols: List of trading symbols (default: top 10 by market cap)
        stream_type: Stream type (trade, bookTicker, depth)
    """

    # Build stream URL
    streams = "/".join([f"{symbol}@{stream_type}@100ms" for symbol in symbols])
    url = f"{BINANCE_WS_URL}/{streams}"

    logger.info(f"Connecting to Binance WebSocket: {url}")
    logger.info(f"Listening for {len(symbols)} symbols on {stream_type} stream")

    try:
        async with websockets.connect(url) as websocket:
            logger.info("Connected!")

            while True:
                message = await websocket.recv(decode=True)
                logger.info(json.dumps(json.loads(message), indent=4))

    except asyncio.CancelledError:
        logger.info("Stream cancelled")
        raise
    except Exception as e:
        logger.error(f"Error during streaming: {e}", exc_info=True)
        raise


def main() -> None:
    """Entry point for the market data ingestion system."""
    try:
        asyncio.run(stream_market_data())
    except KeyboardInterrupt:
        logger.info("Stream stopped by user")


if __name__ == "__main__":
    main()
