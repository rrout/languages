SUBDIRS := dispatchclient  dispatchserver

.PHONY: all $(MAKECMDGOALS) $(SUBDIRS)
$(MAKECMDGOALS) all: $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)
