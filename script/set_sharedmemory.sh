#!/bin/sh

SERVICE=$1
MARKET=$2

# Validate SERVICE
case "$SERVICE" in
    Trade|Quote) ;;
    *)
        echo "Error: Service must be Trade or Quote."
        exit 1
        ;;
esac

# Validate MARKET
case "$MARKET" in
    BTC|TSE) ;;
    *)
        echo "Error: Market must be BTC or TSE."
        exit 1
        ;;
esac

# Switch into script folder
ABSPATH=$(readlink -f "$0")
SCRIPTPATH=$(dirname "$ABSPATH")
cd "$SCRIPTPATH" || exit 1

#----------- By Host Set NUM -----------#
DateCode=$(date +"%-m%d")
SHIFT_BIT=100000

case "$HOSTNAME" in
    server-debug|tm0-hq)
        NUM=$((1020000000000 + DateCode * SHIFT_BIT))
        ;;
    tm1-hq)
        NUM=$((1030000000000 + DateCode * SHIFT_BIT))
        ;;
    tm2-hq)
        NUM=$((1040000000000 + DateCode * SHIFT_BIT))
        ;;
    tm0-ir)
        NUM=$((8000000000000 + DateCode * SHIFT_BIT))
        ;;
    tm1-ir)
        NUM=$((8010000000000 + DateCode * SHIFT_BIT))
        ;;
    tm2-ir)
        NUM=$((8020000000000 + DateCode * SHIFT_BIT))
        ;;
    tm1-hk)
        NUM=$((8030000000000 + DateCode * SHIFT_BIT))
        ;;
    tm2-hk)
        NUM=$((8050000000000 + DateCode * SHIFT_BIT))
        ;;
    *)
        NUM=$((9990000000000 + DateCode * SHIFT_BIT))
        echo "Warning: Unknown host ${HOSTNAME}, using default NUM."
        ;;
esac

#----------- By Service & Market Set KEY -----------#
case "$SERVICE:$MARKET" in
    Trade:BTC) SERIALKEY=11 ;;
    Trade:TSE) SERIALKEY=21 ;;
    Quote:BTC) SERIALKEY=51 ;;
    Quote:TSE) SERIALKEY=61 ;;
esac

BIN_DIR=../bin

"$BIN_DIR/ARSTSetShareNumber" "$SERIALKEY" "$NUM"
echo "Set tick number as $NUM at key $SERIALKEY"

