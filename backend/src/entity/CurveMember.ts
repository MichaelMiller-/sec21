import { Entity, PrimaryGeneratedColumn, Column, ManyToOne, Generated, JoinColumn, OneToOne } from "typeorm";
import { CrossSection } from "./CrossSection";
import { Project } from "./Project";
import { StructuralPoint } from "./StructuralPoint";

export enum SystemLine {
   CENTRE = "centre",
   TOP = "top",
   BOTTON = "buttom",
   LEFT = "left",
   RIGHT = "right",
   TOPLEFT = "top_left",
   TOPRIGHT = "top_right",
   BOTTOMLEFT = "buttom_left",
   BOTTOMRIGHT = "buttom_right"
}

@Entity()
export class CurveMember {

   @PrimaryGeneratedColumn()
   id: number;

   @Column()
   name: string;

   @Column()
   @Generated("uuid")
   uuid: string;

   @OneToOne(type => CrossSection)
   @JoinColumn()
   crossSection: CrossSection;

   @OneToOne(type => StructuralPoint)
   @JoinColumn()
   beginNode: StructuralPoint;

   @OneToOne(type => StructuralPoint)
   @JoinColumn()
   endNode: StructuralPoint;

   @Column({
      type: "enum",
      enum: SystemLine,
      default: SystemLine.CENTRE
   })
   systemLine: SystemLine;

   @ManyToOne(type => Project, project => project.crossSections)
   project: Project;
}