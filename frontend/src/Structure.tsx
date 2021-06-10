import { useEffect, useState } from "react";
import { Spinner, Table, Container, Row, Modal, Form, Button, Col } from "react-bootstrap";
import { RouteComponentProps, useHistory } from "react-router-dom";
import { DbStructuralPoint } from "./DatabaseInterface";
import axios from 'axios';
import CopyButton from "./ui/CopyButton";
import RemoveButton from "./ui/RemoveButton";
import EditButton from "./ui/EditButton";
import CurveMemberList from "./CurveMemberList";
import NewButton from "./ui/NewButton";
import BackButton from "./ui/BackButton";

type StructuralPointsListItemProps = {
   projectId: string;
   id: number;
   name: string;
   uuid: string;
   coordinate_x: number;
   coordinate_y: number;
   coordinate_z: number;
}

function StructuralPointsListItem(props: StructuralPointsListItemProps) {

   const onEdit = () => {
      //! \todo
   }
   const onCopy = () => {
      //! \todo 
   }
   const onDelete = async () => {
      const res = await axios.delete(process.env.REACT_APP_BACKEND + '/structuralpoint/' + props.id)
      if (res.data.success === false) {
         //! \todo handle error
      }
      window.location.reload();
   }
   return (
      <tr>
         <td><EditButton onClick={onEdit} disabled={true}></EditButton></td>
         <td>{props.name}</td>
         <td>{props.coordinate_x}</td>
         <td>{props.coordinate_y}</td>
         <td>{props.coordinate_z}</td>
         <td>{props.uuid}</td>
         <td><CopyButton onClick={onCopy} disabled={true} /></td>
         <td><RemoveButton onClick={onDelete} disabled={false} /></td>
      </tr>
   )
}

type StructuralPointsListProps = {
   projectId: string
   data: DbStructuralPoint[]
}

function StructuralPointsList(props: StructuralPointsListProps) {
   return (
      <>
         <Table borderless hover size="sm">
            <thead>
               <tr>
                  <th></th>
                  <th>Name</th>
                  <th>X</th>
                  <th>Y</th>
                  <th>Z</th>
                  <th>UUID</th>
               </tr>
            </thead>
            <tbody>
               {props.data.map(e => (
                  <StructuralPointsListItem
                     key={e.id}
                     projectId={props.projectId}
                     id={e.id}
                     name={e.name}
                     uuid={e.uuid}
                     coordinate_x={e.coordinate_x}
                     coordinate_y={e.coordinate_y}
                     coordinate_z={e.coordinate_z}
                  />))}
            </tbody>
         </Table>
      </>)
}


type StructuralPointDialogProps = {
   projectId: string;
   show: boolean;
   onHide: () => void;
}

const StructuralPointDialog = (props: StructuralPointDialogProps) => {

   const [data, setData] = useState<DbStructuralPoint>({
      id: 0,
      uuid: "",
      name: "",
      coordinate_x: 0,
      coordinate_y: 0,
      coordinate_z: 0,
   });

   const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
      const { name, value } = e.target;
      setData({ ...data, [name]: value, });
   };

   const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
      event.preventDefault();
      await axios.post(process.env.REACT_APP_BACKEND + '/structuralpoints/' + props.projectId, data)
         .catch((e) => {
            //! \todo show error message
            console.log(e)
         })
      window.location.reload();
   };

   return (
      <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
         <Modal.Header closeButton>
            <Modal.Title id="contained-modal-title-vcenter">Add new structural point</Modal.Title>
         </Modal.Header>
         <Form onSubmit={handleSubmit}>
            <Modal.Body>
               <Form.Row>
                  <Form.Group as={Col} md="4" controlId="validationCustom01">
                     <Form.Label>Name</Form.Label>
                     <Form.Control
                        required
                        type="text"
                        name="name"
                        value={data.name}
                        onChange={handleInputChange}
                     />
                     <Form.Control.Feedback>Please provide a valid name.</Form.Control.Feedback>
                  </Form.Group>
               </Form.Row>
               <Form.Row>
                  <Form.Group as={Col} md="4" controlId="validationCustom02">
                     <Form.Label>X</Form.Label>
                     <Form.Control
                        required
                        type="text"
                        name="coordinate_x"
                        placeholder="X coordinate"
                        value={data.coordinate_x}
                        onChange={handleInputChange}
                     />
                     <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                  </Form.Group>
                  <Form.Group as={Col} md="4" controlId="validationCustom03">
                     <Form.Label>Y</Form.Label>
                     <Form.Control
                        required
                        type="text"
                        name="coordinate_y"
                        placeholder="Y coordinate in UCS"
                        value={data.coordinate_y}
                        onChange={handleInputChange}
                     />
                     <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                  </Form.Group>
                  <Form.Group as={Col} md="4" controlId="validationCustom04">
                     <Form.Label>Z</Form.Label>
                     <Form.Control
                        required
                        type="text"
                        name="coordinate_z"
                        placeholder="Z coordinate in UCS"
                        value={data.coordinate_z}
                        onChange={handleInputChange}
                     />
                     <Form.Control.Feedback>Please provide a valid value.</Form.Control.Feedback>
                  </Form.Group>
               </Form.Row>
            </Modal.Body>
            <Modal.Footer>
               <Button type="submit">Submit</Button>
               <Button onClick={props.onHide}>Close</Button>
            </Modal.Footer>
         </Form>
      </Modal>
   );
}

export type StructureProps = {
   projectId: string
}

const Structure = (props: StructureProps) => {

   const history = useHistory();
   const [structuralPoints, setstructuralPoints] = useState<DbStructuralPoint[]>([])
   const [loading, setLoading] = useState(true)
   const [showNewStructuralPointDialog, setShowNewStructuralPointDialog] = useState(false);

   const onBack = () => { history.push("/project/" + props.projectId); }
   const onNewPoint = () => { history.push("/structuralpoint/" + props.projectId); }

   async function getData() {
      await axios.get(process.env.REACT_APP_BACKEND + '/structuralpoints/' + props.projectId)
         .then(response => setstructuralPoints(response.data))
         .finally(() => setLoading(false))
   }

   useEffect(() => { getData() }, []);

   if (loading) {
      return (<><Spinner animation="border" role="status"></Spinner></>);
   }

   return (
      <Container>
         <BackButton onClick={onBack} disabled={false} />
         <Row>
            <h2>Structural Points</h2>
            <NewButton onClick={() => setShowNewStructuralPointDialog(true)} disabled={false} />
         </Row>
         <StructuralPointsList projectId={props.projectId} data={structuralPoints} />
         <Row>
            <h2>Member</h2>
            <NewButton onClick={onNewPoint} disabled={true} />
         </Row>
         <CurveMemberList projectId={props.projectId} />

         <StructuralPointDialog
            projectId={props.projectId}
            show={showNewStructuralPointDialog}
            onHide={() => setShowNewStructuralPointDialog(false)}
         />
      </Container>
   );
}

export default Structure;
export interface RouterComponentStructureProps extends RouteComponentProps<StructureProps> { }
