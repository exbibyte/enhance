.PHONY: removetemp0
removetemp0:
	find -name '*~' | xargs rm -f

.PHONY: removetemp1
removetemp1:
	find -name '*#' | xargs rm -f

.PHONY: removetemp
removetemp: removetemp0 removetemp1

