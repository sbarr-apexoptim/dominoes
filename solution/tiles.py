from PIL import Image, ImageDraw

class domino:
    w = 0
    h = 0
    r = 25
    cr = 50
    offset = 20
    image = None
    tile = None
    shadow = None
    shadow_draw = None
    draw = None
    def __init__(self, w, h):
        self.r = w / 15
        #self.cr = w / 7.5
        self.cr = w / 5.0
        self.offset = int(w / 20)
#        self.w = w-self.offset
#        self.h = h-self.offset
        self.w = w
        self.h = h
        self.shadow = Image.new('RGBA', (w, h), (255, 255, 255, 255))
        self.tile = Image.new("RGBA", (w, h), (255, 255, 255, 0))
        self.draw = ImageDraw.Draw(self.tile)
        self.draw.chord((0, 0, 2*self.cr, 2*self.cr), start=180, end=270, fill="black")
        self.draw.chord((self.w-2*self.cr, 0, self.w, 2*self.cr), start=270, end=0, fill="black")
        self.draw.chord((0, self.h-2*self.cr, 2*self.cr, self.h), start=90, end=180, fill="black")
        self.draw.chord((self.w-2*self.cr, self.h-2*self.cr, self.w, self.h), start=0, end=90, fill="black")
        self.draw.polygon(((0, self.cr), (self.cr, self.cr), (self.cr, 0)), fill="black")
        self.draw.polygon(((self.w-self.cr, 0), (self.w-self.cr, self.cr), (self.w, self.cr)), fill="black")
        self.draw.polygon(((0, self.h-self.cr), (self.cr, self.h-self.cr), (self.cr, self.h)), fill="black")
        self.draw.polygon(((self.w-self.cr, self.h-self.cr), (self.w, self.h-self.cr), (self.w-self.cr, self.h)), fill="black")
        self.draw.rectangle((self.cr, self.cr, self.w-self.cr, self.h-self.cr), fill="black")
        self.draw.rectangle((self.cr, 0, self.w-self.cr, self.cr), fill="black")
        self.draw.rectangle((self.w-self.cr, self.cr, self.w, self.h-self.cr), fill="black")
        self.draw.rectangle((0, self.cr, self.cr, self.h-self.cr), fill="black")
        self.draw.rectangle((self.cr, self.h-self.cr, self.w-self.cr, self.h), fill="black")
        self.draw.rectangle((0, (self.h/2)-self.w/75, self.w, (self.h/2)+self.w/75), fill="grey")
        
        self.shadow_draw = ImageDraw.Draw(self.shadow)
        self.shadow_draw.chord((0, 0, 2*self.cr, 2*self.cr), start=180, end=270, fill="grey")
        self.shadow_draw.chord((self.w-2*self.cr, 0, self.w, 2*self.cr), start=270, end=0, fill="grey")
        self.shadow_draw.chord((0, self.h-2*self.cr, 2*self.cr, self.h), start=90, end=180, fill="grey")
        self.shadow_draw.chord((self.w-2*self.cr, self.h-2*self.cr, self.w, self.h), start=0, end=90, fill="grey")
        self.shadow_draw.polygon(((0, self.cr), (self.cr, self.cr), (self.cr, 0)), fill="grey")
        self.shadow_draw.polygon(((self.w-self.cr, 0), (self.w-self.cr, self.cr), (self.w, self.cr)), fill="grey")
        self.shadow_draw.polygon(((0, self.h-self.cr), (self.cr, self.h-self.cr), (self.cr, self.h)), fill="grey")
        self.shadow_draw.polygon(((self.w-self.cr, self.h-self.cr), (self.w, self.h-self.cr), (self.w-self.cr, self.h)), fill="black")
        self.shadow_draw.rectangle((self.cr, self.cr, self.w-self.cr, self.h-self.cr), fill="grey")
        self.shadow_draw.rectangle((self.cr, 0, self.w-self.cr, self.cr), fill="grey")
        self.shadow_draw.rectangle((self.w-self.cr, self.cr, self.w, self.h-self.cr), fill="grey")
        self.shadow_draw.rectangle((0, self.cr, self.cr, self.h-self.cr), fill="grey")
        self.shadow_draw.rectangle((self.cr, self.h-self.cr, self.w-self.cr, self.h), fill="grey")
    def add_dots(self, top, bottom):
        if top == 1:
            self.draw.chord((1*self.w/2-self.r, 2*self.h/8-self.r, 1*self.w/2+self.r, 2*self.h/8+self.r), start=0, end=360, fill="white")
        if top == 2:
            self.draw.chord((3*self.w/4-self.r, 1*self.h/8-self.r, 3*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 3*self.h/8-self.r, 1*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
        if top == 3:
            self.draw.chord((3*self.w/4-self.r, 1*self.h/8-self.r, 3*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 2*self.h/8-self.r, 2*self.w/4+self.r, 2*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 3*self.h/8-self.r, 1*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
        if top == 4:
            self.draw.chord((1*self.w/4-self.r, 1*self.h/8-self.r, 1*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 1*self.h/8-self.r, 3*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 3*self.h/8-self.r, 1*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 3*self.h/8-self.r, 3*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
        if top == 5:
            self.draw.chord((1*self.w/4-self.r, 1*self.h/8-self.r, 1*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 1*self.h/8-self.r, 3*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 3*self.h/8-self.r, 1*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 3*self.h/8-self.r, 3*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 2*self.h/8-self.r, 2*self.w/4+self.r, 2*self.h/8+self.r), start=0, end=360, fill="white")
        if top == 6:
            self.draw.chord((1*self.w/4-self.r, 1*self.h/8-self.r, 1*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 1*self.h/8-self.r, 2*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 1*self.h/8-self.r, 3*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 3*self.h/8-self.r, 1*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 3*self.h/8-self.r, 2*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 3*self.h/8-self.r, 3*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
        if top == 7:
            self.draw.chord((1*self.w/4-self.r, 1*self.h/8-self.r, 1*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 1*self.h/8-self.r, 2*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 1*self.h/8-self.r, 3*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 3*self.h/8-self.r, 1*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 3*self.h/8-self.r, 2*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 3*self.h/8-self.r, 3*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 2*self.h/8-self.r, 2*self.w/4+self.r, 2*self.h/8+self.r), start=0, end=360, fill="white")
        if top == 8:
            self.draw.chord((1*self.w/4-self.r, 1*self.h/8-self.r, 1*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 1*self.h/8-self.r, 2*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 1*self.h/8-self.r, 3*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 2*self.h/8-self.r, 1*self.w/4+self.r, 2*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 2*self.h/8-self.r, 3*self.w/4+self.r, 2*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 3*self.h/8-self.r, 1*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 3*self.h/8-self.r, 2*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 3*self.h/8-self.r, 3*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
        if top == 9:
            self.draw.chord((1*self.w/4-self.r, 1*self.h/8-self.r, 1*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 1*self.h/8-self.r, 2*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 1*self.h/8-self.r, 3*self.w/4+self.r, 1*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 2*self.h/8-self.r, 1*self.w/4+self.r, 2*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 2*self.h/8-self.r, 2*self.w/4+self.r, 2*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 2*self.h/8-self.r, 3*self.w/4+self.r, 2*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 3*self.h/8-self.r, 1*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 3*self.h/8-self.r, 2*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 3*self.h/8-self.r, 3*self.w/4+self.r, 3*self.h/8+self.r), start=0, end=360, fill="white")
        if bottom == 1:
            self.draw.chord((1*self.w/2-self.r, 6*self.h/8-self.r, 1*self.w/2+self.r, 6*self.h/8+self.r), start=0, end=360, fill="white")
        if bottom == 2:
            self.draw.chord((3*self.w/4-self.r, 5*self.h/8-self.r, 3*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 7*self.h/8-self.r, 1*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
        if bottom == 3:
            self.draw.chord((3*self.w/4-self.r, 5*self.h/8-self.r, 3*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 6*self.h/8-self.r, 2*self.w/4+self.r, 6*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 7*self.h/8-self.r, 1*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
        if bottom == 4:
            self.draw.chord((1*self.w/4-self.r, 5*self.h/8-self.r, 1*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 5*self.h/8-self.r, 3*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 7*self.h/8-self.r, 1*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 7*self.h/8-self.r, 3*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
        if bottom == 5:
            self.draw.chord((1*self.w/4-self.r, 5*self.h/8-self.r, 1*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 5*self.h/8-self.r, 3*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 7*self.h/8-self.r, 1*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 7*self.h/8-self.r, 3*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 6*self.h/8-self.r, 2*self.w/4+self.r, 6*self.h/8+self.r), start=0, end=360, fill="white")
        if bottom == 6:
            self.draw.chord((1*self.w/4-self.r, 5*self.h/8-self.r, 1*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 5*self.h/8-self.r, 2*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 5*self.h/8-self.r, 3*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 7*self.h/8-self.r, 1*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 7*self.h/8-self.r, 2*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 7*self.h/8-self.r, 3*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
        if bottom == 7:
            self.draw.chord((1*self.w/4-self.r, 5*self.h/8-self.r, 1*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 5*self.h/8-self.r, 2*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 5*self.h/8-self.r, 3*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 7*self.h/8-self.r, 1*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 7*self.h/8-self.r, 2*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 7*self.h/8-self.r, 3*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 6*self.h/8-self.r, 2*self.w/4+self.r, 6*self.h/8+self.r), start=0, end=360, fill="white")
        if bottom == 8:
            self.draw.chord((1*self.w/4-self.r, 5*self.h/8-self.r, 1*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 5*self.h/8-self.r, 2*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 5*self.h/8-self.r, 3*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 6*self.h/8-self.r, 1*self.w/4+self.r, 6*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 6*self.h/8-self.r, 3*self.w/4+self.r, 6*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 7*self.h/8-self.r, 1*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 7*self.h/8-self.r, 2*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 7*self.h/8-self.r, 3*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
        if bottom == 9:
            self.draw.chord((1*self.w/4-self.r, 5*self.h/8-self.r, 1*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 5*self.h/8-self.r, 2*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 5*self.h/8-self.r, 3*self.w/4+self.r, 5*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 6*self.h/8-self.r, 1*self.w/4+self.r, 6*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 6*self.h/8-self.r, 2*self.w/4+self.r, 6*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 6*self.h/8-self.r, 3*self.w/4+self.r, 6*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((1*self.w/4-self.r, 7*self.h/8-self.r, 1*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((2*self.w/4-self.r, 7*self.h/8-self.r, 2*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
            self.draw.chord((3*self.w/4-self.r, 7*self.h/8-self.r, 3*self.w/4+self.r, 7*self.h/8+self.r), start=0, end=360, fill="white")
        #self.image = Image.new('RGBA', (self.w+self.offset, self.h+self.offset), (255, 255, 255, 255))
        #self.image.paste(self.shadow, (0,0), self.shadow)
        #self.image.paste(self.tile, (self.offset,self.offset), self.tile)
        self.image = Image.new('RGBA', (self.w, self.h), (255, 255, 255, 255))
        self.image.paste(self.tile, (0,0), self.tile)
