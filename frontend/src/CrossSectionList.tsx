import { useEffect, useState } from "react";
import { Spinner, Table, Row, Button, Modal, Form, Col, Container, Alert } from "react-bootstrap";
import { DbCrossSection, DbMaterial } from "./DatabaseInterface";
import CopyButton from "./ui/CopyButton";
import EditButton from "./ui/EditButton";
import RemoveButton from "./ui/RemoveButton";
import axios from 'axios';
import NewButton from "./ui/NewButton";

type CrossSectionDialogProps = {
   projectId: string;
   show: boolean;
   onHide: () => void;
}

const CrossSectionDialog = (props: CrossSectionDialogProps) => {
   const [data, setData] = useState<DbCrossSection>({
      id: 0,
      uuid: "",
      name: "",
      area: 0
   });
   const [materialList, setMaterialList] = useState<DbMaterial[]>([]);
   const [loading, setLoading] = useState(true)

   const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
      const { name, value } = e.target;
      setData({ ...data, [name]: value, });
   };

   const handleSubmit = async (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
      event.preventDefault();
      await axios.post(process.env.REACT_APP_BACKEND + '/crosssection/' + props.projectId, data)
         .catch((e) => {
            //! \todo show error message
            console.log(e)
         })
      window.location.reload();
   };

   const getMaterials = async () => {
      await axios.get(process.env.REACT_APP_BACKEND + '/materials/' + props.projectId)
         .then(response => setMaterialList(response.data))
         .finally(() => setLoading(false))
   }

   useEffect(() => { getMaterials() }, []);

   return (
      <Modal {...props} size="lg" aria-labelledby="contained-modal-title-vcenter" centered>
         <Modal.Header closeButton>
            <Modal.Title id="contained-modal-title-vcenter">Add new cross-section</Modal.Title>
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
                  {loading && <Spinner animation="border" role="status"></Spinner>}
                  <Form.Group controlId="ControlSelectBegin">
                     <Form.Label>Material</Form.Label>
                     <Form.Control as="select">
                        {materialList.map(e => (<option key={e.id}>{e.name}</option>))}
                     </Form.Control>
                  </Form.Group>
                  <Form.Group as={Col} md="4" controlId="validationCustom02">
                     <Form.Label>Area [m2]</Form.Label>
                     <Form.Control
                        required
                        type="text"
                        name="area"
                        value={data.area}
                        onChange={handleInputChange}
                     />
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

const CrossSectionListItem = (props: DbCrossSection) => {

   const onEdit = () => {
      //! \todo
   }
   const onCopy = () => {
      //! \todo
   }
   const onDelete = async () => {
      const res = await axios.delete(process.env.REACT_APP_BACKEND + '/crosssection/' + props.id)
      if (res.data.success === false) {
         //! \todo handle error
      }
      window.location.reload();
   }
   return (
      <tr>
         <td><EditButton onClick={onEdit} disabled={true}></EditButton></td>
         <td>{props.name}</td>
         <td></td>
         <td>{props.area}</td>
         <td><CopyButton onClick={onCopy} disabled={true} /></td>
         <td><RemoveButton onClick={onDelete} disabled={false} /></td>
      </tr>
   )
}

export type CrossSectionListProps = {
   projectId: string;
}

const CrossSectionList = (props: CrossSectionListProps) => {

   const [data, setData] = useState<DbCrossSection[]>([])
   const [loading, setLoading] = useState(true)
   const [showNewDialog, setShowNewDialog] = useState(false);

   const getData = async () => {
      await axios.get(process.env.REACT_APP_BACKEND + '/crosssections/' + props.projectId)
         .then(response => setData(response.data))
         .finally(() => setLoading(false))
   }

   useEffect(() => { getData() }, []);

   if (loading) {
      return (<><Spinner animation="border" role="status"></Spinner></>);
   }

   return (
      <Container>
         <Row>
            <h4>Crosssections</h4>
            <NewButton onClick={() => setShowNewDialog(true)} disabled={false} />
         </Row>
         {data.length === 0 && <Alert variant='danger'>No cross section defined.</Alert>}
         <Table borderless hover size="sm">
            <thead>
               <tr>
                  <th></th>
                  <th>Name</th>
                  <th>Material</th>
                  <th>Area [m2]</th>
               </tr>
            </thead>
            <tbody>
               {data.map(e => (
                  <CrossSectionListItem
                     key={e.id}
                     id={e.id}
                     name={e.name}
                     uuid={e.uuid}
                     area={e.area}
                  />))}
            </tbody>
         </Table>

         <CrossSectionDialog
            projectId={props.projectId}
            show={showNewDialog}
            onHide={() => setShowNewDialog(false)}
         />
      </Container>
   )
}

export default CrossSectionList;
