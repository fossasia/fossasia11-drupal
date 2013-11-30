yoxviewSkins["top_menu"] = {
    infoButtons: function(options, lang, sprites, yoxviewPanel, yoxviewContent){
        var $ = jQuery;
        
        //prev/next buttons:
		
        if (!options.skinOptions || options.skinOptions.renderButtons !== false)
        {
            var prevBtn = $("<a>", {
                className: "yoxview_ctlButton yoxview_prevBtn",
                title: lang.PrevImage,
                href: "#"
            });
            prevBtn.click(function(e){
                yoxviewApi.prev();
                return false;
            });
            var nextBtn = $("<a>", {
                className: "yoxview_ctlButton yoxview_nextBtn",
                title: lang.NextImage,
                href: "#"
            });
            nextBtn.click(function(e){
                yoxviewApi.next();
                return false;
            });
            
            yoxviewPanel.append(prevBtn, nextBtn);
        }
        // menu buttons:
        var closeBtn = $("<a>", {
            href: "#",
            title: lang.Close,
            click: function(e){
                e.preventDefault();
                yoxviewApi.close();
            }
        });
		
        closeBtn.append(sprites.getSprite("icons", "close"));
        
        var playBtn = $("<a>", {
            href: "#",
            title: lang.Play,
            click: function(e){
                e.preventDefault();
                yoxviewApi.play();
            }
        });
        playBtn.append(sprites.getSprite("icons", "play"));
        
        var rightBtn = $("<a>", {
            href: "#",
            title: options.isRTL ? lang.PrevImage : lang.NextImage,
            click: function(e){
                e.preventDefault();
                if (options.isRTL)
                    yoxviewApi.prev();
                else
                    yoxviewApi.next();
            }
        });
        rightBtn.append(sprites.getSprite("icons", "right"));
        
        var leftBtn = $("<a>", {
            href: "#",
            title: options.isRTL ? lang.NextImage : lang.PrevImage,
            click: function(e){
                e.preventDefault();
                if (options.isRTL)
                    yoxviewApi.next();
                else
                    yoxviewApi.prev();
            }
        });
        leftBtn.append(sprites.getSprite("icons", "left"));
        
        return {
            close: closeBtn,
            playBtn: playBtn,
            right: rightBtn,
            left: leftBtn
        };
    },
    options: {
        renderInfoExternally: true,
        autoHideInfo: false,
        popupMargin: "50 80 20",
        renderInfoPin: false,
        renderMenu: false,
        renderButtons: false
    }
}

