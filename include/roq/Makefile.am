SUBDIRS = \
	metrics \
	client
AUTOGEN_H = \
        cancel_order.h \
        connection_status.h \
        create_order.h \
        daily_statistics.h \
        download_begin.h \
        download_end.h \
        error.h \
        execution_instruction.h \
        fill.h \
        funds_update.h \
        gateway_status.h \
        layer.h \
        market_by_order.h \
        market_by_price.h \
        market_data_status.h \
        market_status.h \
        mbo_update.h \
        mbp_update.h \
        message_info.h \
        modify_order.h \
        option_type.h \
        order_ack.h \
        order_manager_status.h \
        order_status.h \
        order_type.h \
        order_update.h \
        order_update_action.h \
        origin.h \
        position_effect.h \
        position_update.h \
        reference_data.h \
        request_status.h \
        request_type.h \
        security_type.h \
        session_statistics.h \
        side.h \
        time_in_force.h \
        top_of_book.h \
        trade.h \
        trade_summary.h \
        trade_update.h \
        trading_status.h
CLEANFILES = $(AUTOGEN_H)
BUILT_SOURCES = \
	$(CLEANFILES) \
	.gitignore
pkgincludedir=$(includedir)/roq
pkginclude_HEADERS = \
	$(AUTOGEN_H) \
	account.h \
	api.h \
	compat.h \
	exceptions.h \
	event.h \
	format.h \
	generator.h \
	matcher.h \
	metrics.h \
	span.h \
	string.h \
	subscribe.h \
	user.h \
	utilities.h

.gitignore: Makefile.am
	echo $(AUTOGEN_H) | tr " " "\n" | sort > $@

$(AUTOGEN_H): %.h: ../../schema/%.json
	@abs_top_builddir@/scripts/autogen/main.py \
		--namespace roq \
                --file-type h \
                $< > $@
